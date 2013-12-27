(nu a: 1=>2<String>;
       2=>3<String>;
       Gend)
( // Sender
  link(3,a,s,1);
  s[2] << "Hello";
  end
| // Proxy
  link(3,a,s,2);
  s[1] >> value;
  s[3] << value;
  end
| // Receiver
  link(3,a,s,3);
  s[2] >> value;
  end
)
