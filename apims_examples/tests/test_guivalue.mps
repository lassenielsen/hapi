(nu a: {^Start: {^End: Gend} })
( link(1,a,s,1);
  guisync(1,s,1)
  {^Start(name:String="", value:String=""):
    guivalue(1,s,1,name,value);
    guisync(1,s,1)
    {^End(comment:String=""): end }
  }
)
