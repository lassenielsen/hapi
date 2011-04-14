def X(s1: String) =
  def Y(s2: String) =
    X(s2)
  in Y(s1)
in X("hej")
