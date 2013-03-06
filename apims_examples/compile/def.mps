def X(i:Int) =
  if i<=0
  then end
  else X(i-1)
in
( X(100)
| X(100)
)
