#include <hapi/mpsgui_http.hpp>
#include <dpl/slrparser.hpp>
#include <hapi/mpsparser.hpp>
#include <hapi/common.hpp>
#include "SDL_net.h"
#include "SDL_thread.h"
#include <pthread.h>
#include <string.h>
#include <zlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

using namespace std;
using namespace dpl;
using namespace hapi;

namespace mpsgui
{
// Static Objects
MpsGuiServer maingui;
bool gui_quit=false;
pthread_mutex_t CreateLockedMutex() // {{{
{
  pthread_mutex_t result;
  pthread_mutex_init(&result,NULL);
  pthread_mutex_lock(&result);
  return result;
} // }}}
pthread_mutex_t CreateUnlockedMutex() // {{{
{
  pthread_mutex_t result;
  pthread_mutex_init(&result,NULL);
  return result;
} // }}}
pthread_mutex_t mainlock=CreateUnlockedMutex();
pthread_mutex_t waitlock=CreateLockedMutex();
const string html_template= // {{{
"\
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n\
<html>\n\
<head>\n\
  <title>[[TITLE]]</title>\n\
  <style rel=\"stylesheet\" type=\"text/css\">\n\
    html {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    \n\
    body {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    \n\
    h1, h2, h3, h4, h5, h6 {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    p {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    form {\n\
      padding: 0 0 0 1em;\n\
    }\n\
    input[type=submit][value=accept] {\n\
      background: #292;\n\
    }\n\
    input[type=submit][value=reject] {\n\
      background: #ff0;\n\
    }\n\
  </style>\n\
</head>\n\
<body>\n\
  <a href=\"/\">Home</a>\n\
  <a href=\"#reload\" onClick=\"window.location.reload()\">Reload</a><br/>\n\
  [[BODY]]\
</body>\n\
</html>"; // }}}
const string html_redirect_template= // {{{
"\
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n\
<html>\n\
<head>\n\
  <title>[[TITLE]]</title>\n\
  <meta http-equiv=\"REFRESH\" content=\"1;url=[[DESTINATION]]\">\n\
  <style rel=\"stylesheet\" type=\"text/css\">\n\
    html {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    \n\
    body {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    \n\
    h1, h2, h3, h4, h5, h6 {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    p {\n\
      margin: 0;\n\
      padding: 0;\n\
    }\n\
    form {\n\
      padding: 0 0 0 1em;\n\
    }\n\
    input[type=submit][value=accept] {\n\
      background: #593;\n\
    }\n\
    input[type=submit][value=reject] {\n\
      background: #ff0;\n\
    }\n\
  </style>\n\
</head>\n\
<body>\n\
  [[BODY]]\
</body>\n\
</html>"; // }}}

// Implementing HTTP server
bool ishex(char ch) // {{{
{
  return ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'));
} // }}}
int hexval(char ch) // {{{
{
  if (ch >= '0' && ch <= '9')
    return ch - '0';
  else if (ch >= 'A' && ch <= 'F')
    return 10 + ch - 'A';
  else if (ch >= 'a' && ch <= 'f')
    return 10 + ch - 'a';
  else
    return 0;
} // }}}
string url_decode(string msg) // {{{
{
  string result;
  int pos=0;
  while (pos<msg.size())
  {
    if (msg[pos] == '+') // special case
    { result += ' ';
      pos++;
    }
    else if (msg[pos] == '%' && ishex(msg[pos+1]) && ishex(msg[pos+2])) // encoded char
    { result += (char)(hexval(msg[pos+1])*16 + hexval(msg[pos+2]));
      pos += 3;
    }
    else // unencoded char
      result+=msg[pos++];
  }
  return result;
} // }}}
string extract_possibleid(const parsetree *pid) // {{{
{ if (pid->Type()=="PID" && pid->Case()=="case1") // id
    return pid->Child(0)->Token().Content();
  else if (pid->Type()=="PID" && pid->Case()=="case2") // empty
    return "";
  else
  { cerr << "Error extracting PID content" << endl;
    return "";
  }
} // }}}
void GetData(const parsetree *request, map<string,string> &dest) // {{{
{
  if (request->Type() == "req" && request->Case()=="case1") // GET path
    ;
  else if (request->Type() == "req" && request->Case()=="case2") // POST path datas
    return GetData(request->Child(2),dest);
  else if (request->Type() == "datas" && request->Case()=="case1") // data datas
  {
    string name = url_decode(request->Child(0)->Child(0)->Token().Content());
    string value = url_decode(extract_possibleid(request->Child(0)->Child(2)));
    dest[name] = value;
    return GetData(request->Child(1),dest);
  }
  else if (request->Type() == "datas" && request->Case()=="case2") // data
  {
    string name = url_decode(request->Child(0)->Child(0)->Token().Content());
    string value = url_decode(extract_possibleid(request->Child(0)->Child(2)));
    dest[name] = value;
  }
  else // bad tree
  {
    cerr << "Data error at: " << request->ToString() << endl;
    dest["_ERROR"] = request->ToString();
  }
  return;
} // }}}
map<string,string> GetData(const parsetree *request) // {{{
{
  map<string,string> result;
  result.clear();
  GetData(request,result);
  return result;
} // }}}
void GetPath(const parsetree *request, vector<string> &dest) // {{{
{
  if (request->Type() == "req") // GET path
    return GetPath(request->Child(1),dest);
  else if (request->Type() == "path" && request->Case()=="case1") // / id path
  {
    dest.push_back(request->Child(1)->Token().Content());
    return GetPath(request->Child(2),dest);
  }
  else if (request->Type() == "path" && request->Case()=="case2"); // / brs
  else if (request->Type() == "path" && request->Case()=="case3"); // brs
  else // bad tree
  {
    cerr << "Path error at: " << request->ToString() << endl;
    dest.push_back("_ERROR");
  }
  return;
} // }}}
vector<string> GetPath(const parsetree *request) // {{{
{
  vector<string> result;
  result.clear();
  GetPath(request,result);
  return result;
} // }}}
int min(int x, int y) // {{{
{ return x<y?x:y;
} // }}}
int gzip(const char *input, int size, char *output, int &size_out) // {{{
{ // Compress inout to output buffer.
  int ret, flush;
  unsigned have;
  z_stream strm;
  int pos=0;
  char buf_in[1024];

  /* allocate deflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  ret = deflateInit2(&strm,
                     9,
                     Z_DEFLATED,
                     31, // windowBits between 8 and 15 (Add 16 to specify GZIP header!)
                     9, // memLevel between 1 and 9
                     Z_DEFAULT_STRATEGY);
  if (ret != Z_OK)
    return ret;

  /* compress until end of file */
  do {
    int chunk=min(1024,size-strm.total_in);
    memcpy(buf_in,input+strm.total_in,chunk);
    strm.avail_in = chunk;
    flush = strm.total_in+chunk>=size?Z_FINISH:Z_NO_FLUSH;
    strm.next_in = (Bytef*)buf_in;

    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */
    do {
      strm.avail_out = 4096;
      strm.next_out = (Bytef*)(output+strm.total_out);
      ret = deflate(&strm, flush);    /* no bad return value */
      have = 4096 - strm.avail_out;
    } while (strm.avail_out == 0);
    /* done when last data in file processed */
  } while (flush != Z_FINISH);
  size_out=strm.total_out;
  /* clean up and return */
  (void)deflateEnd(&strm);
  return Z_OK;
} // }}}
void net_send(const string &msg, TCPsocket &dest) // {{{
{
  char *zipdata=new char[msg.size()*2]; // Allocate sufficient space
  int zipsize=0;
  gzip(msg.c_str(),msg.size(),zipdata,zipsize); // Compress message

  stringstream message;
  message.clear();
  message << "HTTP/1.1 200 OK\r\n"
//          << "Date: Thu, 09 Sep 2010 14:00:00 GMT\r\n"
          << "Server: HAPI/2.0\r\n"
//          << "Last-Modified: Thu, 09 Sep 2010 14:00:00 GMT\r\n"
//          << "Accept-Ranges: bytes\r\n"
//          << "Cache-Control: max-age=300, must-revalidate\r\n"
//          << "Expires: Thu, 09 Sep 2010 14:15:00 GMT\r\n"
//          << "Vary: Accept-Encoding\r\n"
          << "Content-Encoding: gzip\r\n"
          << "Content-Length: " << zipsize << "\r\n"
//          << "Keep-Alive: timeout=15\r\n"
//          << "Content-Type: text/html; charset=UTF-8\r\n"
          << "\r\n";
  message.write(zipdata,zipsize);
  // Send zipped message
  string msgstr=message.str();
  SDLNet_TCP_Send(dest, msgstr.c_str(),msgstr.size());

  // Clean up
  delete [] zipdata;
} // }}}
void net_sendfile(const string &filename, TCPsocket &dest) // {{{
{
  string msg="";
  struct stat filestat;
  if (stat(filename.c_str(), &filestat)!=0)
    msg=stringreplace(html_template,"[[FILE NOT FOUND]]",(string)"The file: "+filename+"was not found on the server");
  else
  { char *buffer=new char[filestat.st_size+1];
    ifstream file(filename.c_str(), ios::in | ios::binary);
    file.read(buffer,filestat.st_size);
    file.close();
    for (int i=0;i<filestat.st_size; ++i)
      msg+=buffer[i];
    delete [] buffer;
  }
  net_send(msg,dest);
} // }}}
int server(void *arg) // HTTP Server thread {{{
{
  // Initialise Parser {{{
  SlrParser parser("req");
  parser.DefToken("","[ \t\r][ \t\r]*+[a-zA-Z0-9-][a-zA-Z0-9-]*: [^\n]*\n+HTTP/[1].[1]+.ico HTTP/[1].[1]",9); // Ignore whitespace, except linebreak and browser options
  parser.DefToken("AMP","&",9); // Value Seperator
  parser.DefToken("BR","\n",9); // Linebreak
  parser.DefKeywordToken("POST",1);
  parser.DefKeywordToken("GET",1);
  parser.DefKeywordToken("/",1);
  parser.DefKeywordToken("=",1);
  parser.DefToken("id","[^ \t\r\n/:=&][^ \t\r\n/:=&]*",10);
  parser.DefType("req ::= GET path | POST path datas");
  parser.DefType("path ::= / id path | brs | / brs ");
  parser.DefType("PID ::= id | "); // Possible id
  parser.DefType("data ::= id = PID brs | id = PID AMP");
  parser.DefType("datas ::= data datas | data");
  parser.DefType("brs ::= BR brs | BR");
  // }}}
  // Initialise server {{{
  TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */ 
  IPaddress ip, *remoteIP;
  char buffer[1024];
  if (SDLNet_Init() < 0 || SDLNet_ResolveHost(&ip, NULL, 2000) < 0 || !(sd = SDLNet_TCP_Open(&ip)))
  {
    fprintf(stderr, "SDLNet Error: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
  // }}}
  while (!gui_quit) // Main Loop {{{
  {
    /* This check the sd if there is a pending connection.
     * If there is one, accept that, and open a new socket
     * for communicating */
    if ((csd = SDLNet_TCP_Accept(sd)))
    {
      pthread_mutex_lock(&mainlock);
      /* Now we can communicate with the client using csd socket
       * sd will remain opened waiting other connections */

      int size = SDLNet_TCP_Recv(csd, buffer, 1023);
      buffer[size]=0;
      if (size > 0)
      {
        // Parse Request and get path and post data
        string request = buffer;
        //cout << "HTTP REQUEST:" << endl << request << endl;
        parsetree *request_tree=parser.Parse(request + "\n");
        vector<string> path = GetPath(request_tree);
        map<string,string> data = GetData(request_tree);
        if (path.size()==1 && path[0]=="abc") // ABC MAIN MENU {{{
          net_send(maingui.RenderABC(),csd); // }}}
        else if (path.size()==2 && path[0]=="abc") // USER MENU {{{
          net_send(maingui.RenderABCUser(path[1]),csd); // }}}
        else if (path.size()==3 && path[0]=="abc") // ACTIVITY MENU {{{
          net_send(maingui.RenderABCActivity(path[1],path[2]),csd); // }}}
        else if (path.size()==0) // Main / Select Session {{{
          net_send(maingui.Render(),csd); // }}}
        else if (path.size()==1 && path[0] == "_ERROR") // {{{
        { cerr << "HTTP REQUEST PARSE ERROR!" << endl;
          string result=stringreplace(html_template,"[[TITLE]]",(string)"Request Error");
          result=stringreplace(result,"[[BODY]]",(string)"  <h3>Server Parser Error</h3>\n  <h4>Error:</h4>\n  <p>"+request_tree->ToString()+"</p>\n  <h4>Request:</h4>\n  <p>"+request+"</p>");
          net_send(result,csd);
        } // }}}
        else if (path.size()==2 && path[0] == "data" && path[1]=="logo.jpg") // {{{
        {
          net_sendfile("/opt/hapi/gfx/logo.jpg",csd);
        } // }}}
        else if (path.size()==1 && path[0] == "exit") // {{{
        {
          string result=stringreplace(html_template,"[[TITLE]]",(string)"Server closed");
          result=stringreplace(result,"[[BODY]]",(string)"  <h3>Server Closed</h3>");
          net_send(result,csd);
          gui_quit=1;
        } // }}}
        else if (path.size()==1) // Session / Select Participant {{{
        {
          MpsGuiSession *session=maingui.GetSession(path[0]);
          if (session==NULL)
          { string result=stringreplace(html_template,"[[TITLE]]",(string)"Unknown Session");
            result=stringreplace(result,"[[BODY]]",(string)"  <h4>Session " + path[0] + " does not exist</h4>");
            net_send(result,csd);
          }
          else
            net_send(session->Render(),csd);
        } // }}}
        else if (path.size()==2) // Participant / Choices {{{
        {
          MpsGuiSession *session=maingui.GetSession(path[0]);
          if (session==NULL)
          { string result=stringreplace(html_template,"[[TITLE]]",(string)"Unknown Session");
            result=stringreplace(result,"[[BODY]]",(string)"  <h4>Session " + path[0] + " does not exist</h4>");
            net_send(result,csd);
          }
          else
          {
            MpsGuiParticipant *participant=session->GetParticipant(string2int(path[1]));
            if (participant==NULL)
            { string result=stringreplace(html_template,"[[TITLE]]",(string)"Unknown Participant");
              result=stringreplace(result,"[[BODY]]",(string)"  <h4>Participant " + path[1] + " does not exist in Session " + path[0] + "</h4>");
              net_send(result,csd);
            }
            else
              net_send(participant->Render(),csd);
          }
        } // }}}
        else if (path.size()==3 && path[2]=="update") // Update Participant / Redirect {{{
        {
          MpsGuiSession *session=maingui.GetSession(path[0]);
          if (session==NULL)
          { string result=stringreplace(html_template,"[[TITLE]]",(string)"Unknown Session");
            result=stringreplace(result,"[[BODY]]",(string)"  <h4>Session " + path[0] + " does not exist</h4>");
            net_send(result,csd);
          }
          else
          {
            MpsGuiParticipant *participant=session->GetParticipant(string2int(path[1]));
            if (participant==NULL)
            { string result=stringreplace(html_template,"[[TITLE]]",(string)"Unknown Participant");
              result=stringreplace(result,"[[BODY]]",(string)"  <h4>Participant " + path[1] + " does not exist in Session " + path[0] + "</h4>");
              net_send(result,csd);
            }
            else
            {
              participant->UpdateChoice(data);
              pthread_mutex_unlock(&waitlock);
              string html = stringreplace(html_redirect_template,"[[TITLE]]","Redirecting");
              html = stringreplace(html,"[[DESTINATION]]",(string)"/"+path[0]+"/"+path[1]+"/");
              html = stringreplace(html,"[[BODY]]","Wait for redirection...");
              net_send(html,csd);
            }
          }
        } // }}}
        else // {{{
        { cerr << "HTTP REQUEST UNKNOWN PATH!" << endl;
          string result=stringreplace(html_template,"[[TITLE]]",(string)"Request Error");
          string body="  <h3>Unknown Path</h3>\n  <h4>Path:</h4>\n  <p>";
          for (vector<string>::const_iterator it=path.begin(); it!=path.end(); ++it)
            body+="/"+(*it);
          body+="</p>\n  <h4>Request:</h4>\n  <p>"+request+"</p>";
          result=stringreplace(result,"[[BODY]]",body);
          net_send(result,csd);
        } // }}}
        // Clean up
        delete request_tree;
        request_tree=0;
      }
      /* Close the client socket */
      pthread_mutex_unlock(&mainlock);
      SDLNet_TCP_Close(csd);
    }
    else // No Connection: Wait a bit
      usleep(100000);
  } // }}}
  SDLNet_TCP_Close(sd);
  SDLNet_Quit();
  pthread_mutex_unlock(&waitlock);
  return EXIT_SUCCESS;
} // }}}
// start server (Since thread not deallocated, this is a memmory leak)
SDL_Thread *serverthread=SDL_CreateThread(server, &maingui);

// Implementing MPSGUI API
void CreateSession(const std::string &session, int maxpid) // {{{
{
  pthread_mutex_lock(&mainlock);
  maingui.RegisterSession(session,maxpid);
  pthread_mutex_unlock(&mainlock);
} // }}}
void ClearChoices(const std::string &session) // {{{
{
  pthread_mutex_lock(&mainlock);
  maingui.ClearChoices(session);
  pthread_mutex_unlock(&mainlock);
} // }}}
void SetChoices(const std::string &session, // {{{
                int pid,
                const std::vector<Choice> &choices)
{
  pthread_mutex_lock(&mainlock);
  maingui.SetChoices(session,pid,choices);
  pthread_mutex_unlock(&mainlock);
} // }}}
std::vector<Choice> GetChoices(const std::string &session, int pid) // {{{
{
  pthread_mutex_lock(&mainlock);
  vector<Choice> result=maingui.GetChoices(session,pid);
  pthread_mutex_unlock(&mainlock);
  return result;
} // }}}
bool GuiWait() // {{{
{
  pthread_mutex_lock(&waitlock);
  return not gui_quit;
} // }}}
bool GuiEnabeled() // {{{
{
  return not gui_quit;
} // }}}
void SetValue(const std::string &session, // {{{
              int pid,
              const std::string &name,
              const std::string &value)
{
  pthread_mutex_lock(&mainlock);
  maingui.SetValue(session,pid,name,value);
  pthread_mutex_unlock(&mainlock);
} // }}}
void Show() // {{{
{
} // }}}
void Hide() // {{{
{
} // }}}

// MpsGuiServer
MpsGuiServer::MpsGuiServer() // {{{
{
  mySessions.clear();
} // }}}
MpsGuiServer::~MpsGuiServer() // {{{
{
  for (map<string,MpsGuiSession*>::const_iterator it=mySessions.begin(); it!=mySessions.end(); ++it)
    delete it->second;
} // }}}
void MpsGuiServer::RegisterSession(const string &session, int maxpid) // {{{
{
  if (mySessions.find(session)!=mySessions.end())
    delete mySessions[session];
  mySessions[session]=new MpsGuiSession(session,maxpid);
} // }}}
void MpsGuiServer::ClearChoices(const string &session) // {{{
{
  if (mySessions.find(session) != mySessions.end())
    mySessions[session]->ClearChoices();
} // }}}
void MpsGuiServer::SetChoices(const string &session, int pid, const vector<Choice> &choices) // {{{
{
  if (mySessions.find(session) != mySessions.end())
    mySessions[session]->SetChoices(pid,choices);
} // }}}
vector<Choice> MpsGuiServer::GetChoices(const string &session,int pid) // {{{
{
  vector<Choice> result;
  result.clear();
  if (mySessions.find(session) != mySessions.end())
    result=mySessions[session]->GetChoices(pid);
  return result;
} // }}}
MpsGuiSession *MpsGuiServer::GetSession(const string &name) // {{{
{
  if (mySessions.find(name) == mySessions.end())
    return NULL;
  return mySessions[name];
} // }}}
void MpsGuiServer::SetValue(const string &session, int pid, const string &name, const string &value) // {{{
{
  if (mySessions.find(session) != mySessions.end())
    mySessions[session]->SetValue(pid,name,value);
} // }}}
string MpsGuiServer::Render() // {{{
{
  // Page template
  string result=stringreplace(html_template,"[[TITLE]]","Session List");
  result=stringreplace(result,"[[BODY]]","  <h4>Sessions:</h4>\n  <p>\n[[SESSIONS]]  </p>\n  <p>\n    <a href=\"/exit/\">Stop server</a>\n  </p>");
  for (map<string,MpsGuiSession*>::const_iterator it=mySessions.begin(); it!=mySessions.end(); ++it)
    result=stringreplace(result,"[[SESSIONS]]",(string)"    <a href=\"/" + it->first + "/\">" + it->first + "</a><br/>\n[[SESSIONS]]");
  result=stringreplace(result,"[[SESSIONS]]","");
  return result;
} // }}}
string MpsGuiServer::RenderABC() // {{{
{
  // Find active users
  set<string> users;
  for (map<string,MpsGuiSession*>::const_iterator session=mySessions.begin(); session!=mySessions.end(); ++session)
  { for (int pid=0; pid<session->second->GetMaxpid(); ++pid)
    { string user = session->second->GetValue(pid,"User");
      if (user.size()>0 && user[0]!='_')
       users.insert(user);
    }
  }
  // Page template
  string result=stringreplace(html_template,(string)"[[TITLE]]","ABC: Select User");
  // Set page structure
  result=stringreplace(result,"[[BODY]]","  <h4>Users:</h4>\n  <p>\n[[USERS]]  </p>");
  for (set<string>::const_iterator user=users.begin(); user!=users.end(); ++user)
  { result=stringreplace(result,"[[USERS]]",(string)"    <a href=\"/abc/" + *user + "/\">" + *user + "</a><br/>\n[[USERS]]");
  }
  // Remove position tags from page
  result=stringreplace(result,"[[USERS]]","");
  return result;
} // }}}
string MpsGuiServer::RenderABCUser(const string &user) // {{{
{
// FIXME  // Find possible activities
// FIXME  set<string> activities;
// FIXME  for (map<string,MpsGuiSession*>::const_iterator session=mySessions.begin(); session!=mySessions.end(); ++session)
// FIXME  { for (int pid=0; pid<session->second->GetMaxpid(); ++pid)
// FIXME    { string session_user = session->second->GetValue(pid,"User");
// FIXME      if (session_user == user)
// FIXME      { vector<Choice> participant=session->second->GetParticipant(pid)->GetChoices();
// FIXME        for (vector<Choice>::const_iterator choice=choices.begin(); choice!=choices.end(); ++choice)
// FIXME        { string action=choice.name;
// FIXME          int startwriter=action.find("_",0);
// FIXME          int endwriter=action.find("_",0);
// FIXME          string writer = activity.substr(startwriter+1,endwriter-startwriter-1)
// FIXME          cout << "Finding Activities:" << endl
// FIXME               << "Action: " << action << endl
// FIXME               << "writerpos: " << startwriter << "-" << endwriter << endl;
// FIXME          for (int startpos=endwriter,int endpos=action.find("_",startpos);
// FIXME               startpos!=string::npos;
// FIXME               startpos=endpos,endpos=action.find("_",startpos))
// FIXME          { string activity=action.substr(startpos+1,endpos-startpos-1);
// FIXME            activities.push_back(activity);
// FIXME          }
// FIXME        }
// FIXME      }
// FIXME    }
// FIXME  }
// FIXME  // Page template
  string result=stringreplace(html_template,(string)"[[TITLE]]","ABC - User: "+user);
  result=stringreplace(result,"[[BODY]]","  <h4>User: [[USER]]</h4>\n  <h4>Activities:</h4>\n  <p>\n[[ACTIVITIES]]  </p>");
  // Set USER field
  result=stringreplace(result,"[[USER]]",user);
  // Fill ACTIVITIES section
// FIXME  for (set<string>::const_iterator user=users.begin(); user!=users.end(); ++user)
// FIXME  { result=stringreplace(result,"[[ACTIVITIES]]",(string)"    <a href=\"/abc/" + *user + "/\">" + *user + "</a><br/>\n[[USERS]]");
// FIXME  }
// FIXME  result=stringreplace(result,"[[USERS]]","");
// FIXME  cout << "Sending page: " << result << endl;
  return result;
} // }}}
string MpsGuiServer::RenderABCActivity(const string &user, const string &activity) // {{{
{
  // Page template
  string result=stringreplace(html_template,(string)"[[TITLE]]","ABC - User: "+user+", Activity: "+activity);
//  set<string> users;
//  for (map<string,MpsGuiSession*>::const_iterator session=mySessions.begin(); session!=mySessions.end(); ++session)
//  {
//  }
//  result=stringreplace(result,"[[BODY]]","  <h4>Users:</h4>\n  <p>\n[[SESSIONS]]  </p>\n  <p>\n    <a href=\"/exit/\">Stop server</a>\n  </p>");
//  for (map<string,MpsGuiSession*>::const_iterator it=mySessions.begin(); it!=mySessions.end(); ++it)
//    result=stringreplace(result,"[[SESSIONS]]",(string)"    <a href=\"/" + it->first + "/\">" + it->first + "</a><br/>\n[[SESSIONS]]");
//  result=stringreplace(result,"[[SESSIONS]]","");
  return result;
} // }}}

// MpsGuiSession
MpsGuiSession::MpsGuiSession(const string &name, int maxpid) // {{{
: myName(name),
  myMaxpid(maxpid)
{
  for (int i=0; i<myMaxpid; ++i)
    myParticipants.push_back(new MpsGuiParticipant(name, i));
} // }}}
MpsGuiSession::~MpsGuiSession() // {{{
{
  while (myParticipants.size()>0)
  {
    delete (*myParticipants.begin());
    myParticipants.erase(myParticipants.begin());
  }
} // }}}
void MpsGuiSession::ClearChoices() // {{{
{
  for (int i=0; i<myParticipants.size(); ++i)
    myParticipants[i]->ClearChoices();
} // }}}
void MpsGuiSession::SetChoices(int pid, const vector<Choice> &choices) // {{{
{
  if (pid-1 < myParticipants.size())
    myParticipants[pid-1]->SetChoices(choices);
} // }}}
vector<Choice> MpsGuiSession::GetChoices(int pid) // {{{
{
  vector<Choice> result;
  result.clear();
  if (pid-1 < myParticipants.size())
    result = myParticipants[pid-1]->GetChoices();
  return result;
} // }}}
MpsGuiParticipant *MpsGuiSession::GetParticipant(int pid) // {{{
{
  if (pid < myParticipants.size())
    return myParticipants[pid];
  return NULL;
} // }}}
void MpsGuiSession::SetValue(int pid, const string &name, const string &value) // {{{
{
  if (pid-1 < myParticipants.size())
    myParticipants[pid-1]->SetValue(name,value);
} // }}}
string MpsGuiSession::GetValue(int pid, const string &name) // {{{
{
  if (pid-1 < myParticipants.size())
    return myParticipants[pid-1]->GetValue(name);
  else
    return "_ERROR: No such participant";
} // }}}
int MpsGuiSession::GetMaxpid() // {{{
{ return myMaxpid;
} // }}}
string MpsGuiSession::Render() // {{{
{
  string result=stringreplace(html_template,"[[TITLE]]",(string)"Session: " + myName);
  result = stringreplace(result,"[[BODY]]","  <h4>Participants:</h4>\n[[PARTICIPANTS]]");
  for (int i=0; i<myMaxpid; ++i)
    result=stringreplace(result,"[[PARTICIPANTS]]",(string)"  <a href=\"/" + myName + "/" + int2string(i) + "/\">" + int2string(i) + "</a><br/>\n[[PARTICIPANTS]]");
  result = stringreplace(result,"[[PARTICIPANTS]]","");
  return result;
} // }}}

// MpsGuiParticipant
MpsGuiParticipant::MpsGuiParticipant(const string &session, int pid) // {{{
{
  myPid = pid;
  mySession = session;
  myChoices.clear();
  myValues.clear();
} // }}}
MpsGuiParticipant::~MpsGuiParticipant() // {{{
{
} // }}}
void MpsGuiParticipant::ClearChoices() // {{{
{
  myChoices.clear();
} // }}}
void MpsGuiParticipant::SetChoices(const vector<Choice> &choices) // {{{
{
  // Save choices
  myChoices=choices;
  // Set default arguments
  for (int choice=0; choice<myChoices.size(); ++choice)
    for (int arg=0; arg<myChoices[choice].args.size(); ++arg)
    {
      map<string,string>::const_iterator val=myValues.find(myChoices[choice].args[arg].name);
      if (val != myValues.end())
      {
        delete myChoices[choice].args[arg].value;
        myChoices[choice].args[arg].value = hapi::MpsParser::Exp(val->second);
      }
    }
} // }}}
vector<Choice> MpsGuiParticipant::GetChoices() // {{{
{
  return myChoices;
} // }}}
void MpsGuiParticipant::AcceptChoice(const string &choice) // {{{
{
  for (vector<Choice>::iterator it=myChoices.begin(); it!=myChoices.end(); ++it)
    if (it->name == choice)
      it->accept=true;
} // }}}
void MpsGuiParticipant::RejectChoice(const string &choice) // {{{
{
  for (vector<Choice>::iterator it=myChoices.begin(); it!=myChoices.end(); ++it)
    if (it->name == choice)
      it->accept=false;
} // }}}
bool MpsGuiParticipant::SetChoiceValue(const string &choice, const string &arg, const string &value) // {{{
{
  bool result = false;
  bool cont = true;
  for (vector<Choice>::iterator it=myChoices.begin(); cont && it!=myChoices.end(); ++it)
    if (it->name == choice)
      for (vector<ChoiceArg>::iterator it2=it->args.begin(); cont && it2!=it->args.end(); ++it2)
        if (it2->name == arg)
        { hapi::MpsExp *exp=NULL;
          if (typeid(*it2->type)==typeid(MpsStringMsgType)) // Create string value
            exp=new MpsStringVal(stuff_string(value));
          else
            exp=hapi::MpsParser::Exp(value);
          hapi::MpsMsgEnv Gamma;
          Gamma.clear();
          MpsMsgType *inputtype=exp->TypeCheck(Gamma);
          bool inputtypematch=inputtype->Equal(MpsBoolVal(true),*it2->type);
          delete inputtype;
          if (inputtypematch)
          {
            delete it2->value;
            it2->value = exp;
            result = true;
            cont = false;
          }
          else
          {
            delete exp;
            result = false;
            cont = false;
          }
        }
  return result;
} // }}}
void MpsGuiParticipant::ClearBoolChoices(const string &choice) // {{{
{
  for (vector<Choice>::iterator it=myChoices.begin(); it!=myChoices.end(); ++it)
    if (it->name == choice)
      for (vector<ChoiceArg>::iterator it2=it->args.begin(); it2!=it->args.end(); ++it2)
        if (it2->type->ToString() == "Bool")
        {
          delete it2->value;
          it2->value = new MpsBoolVal(false);
        }
} // }}}
void MpsGuiParticipant::SetValue(const string &name, const string &value) // {{{
{
  myValues[name]=value;
} // }}}
string MpsGuiParticipant::GetValue(const string &name) // {{{
{
  map<string,string>::const_iterator value = myValues.find(name);
  if (value==myValues.end())
    return "_ERROR: Value undefined";
  else
    return value->second;
} // }}}
void MpsGuiParticipant::UpdateChoice(const map<string,string> &data) // {{{
{
  bool accept = false;
  bool reject = false;
  map<string,string>::const_iterator choice=data.find("choice");
  if (choice != data.end())
  {
    ClearBoolChoices(choice->second);
    for (map<string,string>::const_iterator it=data.begin(); it!=data.end(); ++it)
      if (it->first == "choice")
        ;
      else if (it->first == "accept")
        accept=true;
      else if (it->first == "reject")
        reject=true;
      else
        if (not SetChoiceValue(choice->second,it->first,it->second))
          reject=true; // Do not accept
    if (accept && not reject)
      AcceptChoice(choice->second);
    else
      RejectChoice(choice->second);
  }
} // }}}
string MpsGuiParticipant::Render() // {{{
{
  string result;
  if (myChoices.size()>0)
    result = html_template;
  else
    result = stringreplace(html_redirect_template,"[[DESTINATION]]",(string)"/"+mySession+"/"+int2string(myPid));
  result = stringreplace(result,"[[TITLE]]",(string)"Participant: " + int2string(myPid));
  result = stringreplace(result,"[[BODY]]","  <h3>Data:</h3>\n[[DATA]]\n  <h3>Choices:</h3>\n[[CHOICES]]");

  for (map<string,string>::const_iterator it=myValues.begin(); it!=myValues.end(); ++it)
    result = stringreplace(result,"[[DATA]]",(string)"    " + it->first + "=" + it->second + "<br/>\n[[DATA]]");
  result = stringreplace(result,"[[DATA]]","");
  for (int choice=0; choice<myChoices.size(); ++choice)
  {
    result = stringreplace(result,"[[CHOICES]]",(string)"  <h4>" + myChoices[choice].name + "</h4>\n  <form action=\"/[[SESSION]]/[[PID]]/update/\" method=\"post\">\n    <input type=\"hidden\" name=\"choice\" value=\"[[CHOICE]]\"/>\n[[ARGS]]  </form>\n[[CHOICES]]");
    result=stringreplace(result,"[[SESSION]]",mySession);
    result=stringreplace(result,"[[PID]]",int2string(myPid));
    result=stringreplace(result,"[[CHOICE]]",myChoices[choice].name);
    for (int arg=0; arg<myChoices[choice].args.size(); ++arg)
    { if (typeid(*myChoices[choice].args[arg].type)==typeid(MpsIntMsgType))
        result = stringreplace(result,"[[ARGS]]",(string)"    " + myChoices[choice].args[arg].name + ": <input name=\"" + myChoices[choice].args[arg].name + "\" type=\"string\" value=\""
               + myChoices[choice].args[arg].value->ToString()
               + "\"/><br/>\n[[ARGS]]");
      else if (typeid(*myChoices[choice].args[arg].type)==typeid(MpsBoolMsgType))
      { bool value = myChoices[choice].args[arg].value->ToString()=="true";
        result = stringreplace(result,"[[ARGS]]",(string)"    " + myChoices[choice].args[arg].name + ": <input name=\"" + myChoices[choice].args[arg].name + "\" type=\"checkbox\" value=\"true\" "
               + (value?"checked":"") + "/><br/>\n[[ARGS]]");
      }
      else
      { string value = unwrap_string(myChoices[choice].args[arg].value->ToString());
        result = stringreplace(result,"[[ARGS]]",(string)"    " + myChoices[choice].args[arg].name + ": <input name=\"" + myChoices[choice].args[arg].name + "\" type=\"string\" value=\""
               + stringreplace(stringreplace(value,"&","&amp;"),"\"","&quot;")
               + "\"/><br/>\n[[ARGS]]");
      }
    }
    result = stringreplace(result,"[[ARGS]]","    <input type=\"submit\" name=\"accept\" value=\"accept\"/><br/>\n[[ARGS]]");
    if (myChoices[choice].accept)
      result = stringreplace(result,"[[ARGS]]","    <input type=\"submit\" name=\"reject\" value=\"reject\"/><br/>\n[[ARGS]]");
    result = stringreplace(result,"[[ARGS]]","");
  }
  result = stringreplace(result,"[[CHOICES]]","");
  return result;
} // }}}
}
