(nu a: 1=>3:1<Bool>;
       2=>3:2<Bool>;
       3=>1:3<String>;
       3=>2:4<String>;
       Gend)
( // The judge
  link(3,a,s,3);
  s[1] >> v1;
  s[2] >> v2;
  if (v1 and not v2) or (v2 and not v1) // Has player 2 won
  then
  ( s[3] << "Sorry, player 2 has won";
    s[4] << "Congratulations";
    end
  )
  else
  ( s[3] << "Congratulations";
    s[4] << "Sorry, player 1 has won";
    end
  )
| // Player 1
  link(3,a,s,1);
  s[1] << true;
  s[3] >> x;
  end
| // Player 2
  link(3,a,s,2);
  s[2] << false;
  s[4] >> x;
  end
)
