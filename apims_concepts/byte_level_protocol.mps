// This concept code serves to illustrate he ideas to declaring the byte-level protocol
// by this we mean the bytes that are actually sent when selection and branching are performed
grammar
  // First define tokens
  token EOL="(\n|\r\n|\n\r)[ \t\r\n]*",
  token ="[ \t]+", // Whitespace token def, can be ignored in grammar
  token GET="(G|g)(E|e)(T|t)",
  token PUT="(P|p)(U|u)(T|t)",
  token OPTION="|-R|-r",
  token LOCATION="(|/)([a-zA-Z.]+/)*[a-zA-Z.]+",
  token QUIT="(Q|q)(U|u)(I|i)(T|t)",
  more tokens ...
  bnf   CMD = GET OPTION LOCATION EOL |
              PUT OPTION LOCATION EOL |
              more operations ,
  bnf   CONNECTION = (CMD REPLY)* QUIT EOL
in
  
define $ftp = 1=>2:1<CMD>
  binary:  // binary requires only 2 participants,
           // and simplifies the protocol used to use only a single channel
           // to improve efficiency and support many common network protocols
  1=>2:2
  {^GET identified by grammar "GET" app: // identified by used to declare bytes sent when this choice is selected,
                                              // and app means that the bytes can simply be appended to the front of the first message after the choice
    1=>2:2<String>; // Send path to file or dir
    2=>1:1<String>;
