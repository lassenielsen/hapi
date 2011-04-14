// Test if unused metods are removed from the environment
def X(x: Int) =
  def Y(y : Int) = X(x-y)
  in if x=0 then end else Y(1)
in X(1000)
