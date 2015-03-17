def Rec<equal : Bool>(val : Int) =
  Rec<not equal>(val+1)
in Rec<true>(0)
