// Journal {{{
// Jounal Interface {{{
// Deltagere:
// 1: User
// 2: Journal
define $journal xIndskrevet
                xUrinering xAffoering
                xOperation xInformeret xReserveret 
                xMedicinMorgen xMedicinEftermiddag xMedicinAften =
  rec $actions<xIndskrevet:Bool=xIndskrevet,
               xUrinering:Bool=xUrinering,
               xAffoering:Bool=xAffoering,
               xOperation:Bool=xOperation,
               xInformeret:Bool=xInformeret,
               xReserveret:Bool=xReserveret,
               xMedicinMorgen:Bool=xMedicinMorgen,
               xMedicinEftermiddag:Bool=xMedicinEftermiddag,
               xMedicinAften:Bool=xMedicinAften>.
    1=>2:1
    {^LaesNavn[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivNavn[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesCPR[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivCPR[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesVaerelse[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivVaerelse[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesVaeskeindtag[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivVaeskeindtag[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesEnergiindtag[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivEnergiindtag[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesSymptomer[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivSymptomer[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesAftale[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivAftale[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^LaesMedicin[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivMedicin[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivUrinering[[xIndskrevet]]:
      1=>2:1<Bool> as xUrinering;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivAffoering[[xIndskrevet]]:
      1=>2:1<Bool> as xAffoering;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivOperation[[xIndskrevet]]:
      1=>2:1<Bool> as xOperation;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivInformeret[[xIndskrevet]]:
      1=>2:1<Bool> as xInformeret;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivReserveret[[xIndskrevet]]:
      1=>2:1<Bool> as xReserveret;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivMedicinering[[xIndskrevet]]:
      1=>2:1<Bool> as xMedicinMorgen;
      1=>2:1<Bool> as xMedicinEftermiddag;
      1=>2:1<Bool> as xMedicinAften;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SkrivIndskrevet:
      1=>2:1<Bool> as xIndskrevet;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^Arkiver[[not xIndskrevet]]: Gend
    }
in // }}}
// Opret Kanal: journal {{{
(nu journal :
  1=>2:1<String>; // Set Navn 
  1=>2:1<String>; // Set CPR
  1=>2:1<String>; // Set Vaerelse
  1=>2:1<String>; // Set Symptoms
  $journal<true,false,false,false,false,false,false,false,false>) // }}}
( // Journal Service Implementation {{{
  def Journal() =
    link(2,journal,s,2);
    ( Journal()
    | s[1]>>navn;
      s[1]>>cpr;
      s[1]>>room;
      s[1]>>symptomer;
      def State<pIndskrevet:Bool, pUrinering:Bool, pAffoering:Bool,
                pOperation:Bool, pInformeret:Bool, pReserveret:Bool,
                pMedicinMorgen:Bool,pMedicinEftermiddag:Bool,
                pMedicinAften:Bool>
               (s:$journal<pIndskrevet,pUrinering,pAffoering,
                           pOperation,pInformeret,pReserveret,
                           pMedicinMorgen,pMedicinEftermiddag,
                           pMedicinAften>@(2of2),
                pNavn:String, pCPR:String, pVaerelse:String,
                pVaeskeindtag:String, pEnergiindtag:String,
                pSymptomer:String, pAftale:String, pMedicin:String) =
        s[1]>>
        {^LaesNavn:
          s[2]<<pNavn;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^SkrivNavn:
          s[1]>>nytNavn;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,nytNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^LaesCPR:
          s[2]<<pCPR;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^SkrivCPR:
          s[1]>>nytCPR;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,nytCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^LaesVaerelse:
          s[2]<<pCPR;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^SkrivVaerelse:
          s[1]>>nytVaerelse;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,nytVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^LaesVaeskeindtag:
          s[2]<<pVaeskeindtag;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^SkrivVaeskeindtag:
          s[1]>>nytVaeskeindtag;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,nytVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^LaesEnergiindtag:
          s[2]<<pEnergiindtag;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^SkrivEnergiindtag:
          s[1]>>nytEnergiindtag;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,nytEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^LaesSymptomer:
          s[2]<<pSymptomer;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^SkrivSymptomer:
          s[1]>>nyeSymptomer;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,nyeSymptomer,pAftale,pMedicin),
         ^LaesAftale:
          s[2]<<pAftale;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivAftale:
          s[1]>>nyAftale;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,nyAftale,pAftale,pMedicin),
         ^LaesMedicin:
          s[2]<<pMedicin;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pMedicin,pMedicin,pMedicin),
         ^SkrivMedicin:
          s[1]>>nyMedicin;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,nyMedicin,pMedicin,pMedicin),
         ^SkrivUrinering:
          s[1]>>nyUrinering;
          State<pIndskrevet,nyUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivAffoering:
          s[1]>>nyAffoering;
          State<pIndskrevet,pUrinering,nyAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivOperation:
          s[1]>>nyOperation;
          State<pIndskrevet,pUrinering,pAffoering,nyOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivInformeret:
          s[1]>>nyInformeret;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,nyInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivReserveret:
          s[1]>>nyReserveret;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,nyReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivMedicinering:
          s[1]>>nyMedicinMorgen;
          s[1]>>nyMedicinEftermiddag;
          s[1]>>nyMedicinAften;
          State<pIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,nyMedicinMorgen,nyMedicinEftermiddag,nyMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pAftale,pAftale,pMedicin),
         ^SkrivIndskrevet:
          s[1]>>nyIndskrevet;
          State<nyIndskrevet,pUrinering,pAffoering,pOperation,pInformeret,pReserveret,pMedicinMorgen,pMedicinEftermiddag,pMedicinAften>
               (s,pNavn,pCPR,pVaerelse,pVaeskeindtag,pEnergiindtag,pSymptomer,pAftale,pMedicin),
         ^Arkiver: end
        }
      in State<true,false,false,false,false,false,false,false,false>(s,navn,cpr,room,"(unknown)","(unknown)",symptomer,"Ingen","Ingen")
    )
  in Journal() | // }}}
// }}}
// Sekretær {{{
// Sekretær Interface  // {{{
// Deltagere:
// 1: Sygeplejer
// 2: Sekretær
define $hent_journal xUrinering xAffoering xOperation xInformeret xReserveret xMedicinMorgen xMedicinEftermiddag xMedicinAften =
  2=>1:2<$journal<true,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2)>;
  Gend
in
define $sekretaer =
  1=>2:1<Bool> as xUrinering;
  1=>2:1<Bool> as xAffoering;
  1=>2:1<Bool> as xOperation;
  1=>2:1<Bool> as xInformeret;
  1=>2:1<Bool> as xReserveret;
  1=>2:1<Bool> as xMedicinMorgen;
  1=>2:1<Bool> as xMedicinEftermiddag;
  1=>2:1<Bool> as xMedicinAften;
  1=>2:1<$journal<true,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2)>;
  $hent_journal<xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>
in // }}}
// Opret Kanal: sekretaer // {{{
(nu sekretaer: $sekretaer) // }}}
( // Sekretær Service Implementation {{{
  def Sekretaer() =
    link(2,sekretaer,s,2);
    ( Sekretaer()
    | s[1]>>xUrinering;
      s[1]>>xAffoering;
      s[1]>>xOperation;
      s[1]>>xInformeret;
      s[1]>>xReserveret;
      s[1]>>xMedicinMorgen;
      s[1]>>xMedicinEftermiddag;
      s[1]>>xMedicinAften;
      s[1]>>j;
      // FIXME: Skriv opdateringer ind i journalen
      s[2]<<j;
      end
    )
  in
    Sekretaer() | // }}}
// }}}
// Behandling {{{
// Behandlings Interface {{{
// Deltagere:
// 1: Patient
// 2: Læge
// 3: Sygeplejer
define $2114 xUrinering xAffoering
             xUdskriv
             xJournalHer xJournalOpdateret
             xOperation xInformeret xReserveret
             xMedicinMorgen xMedicinEftermiddag xMedicinAften =
  rec $treatment<xUrinering:Bool=xUrinering,
                 xAffoering:Bool=xAffoering,
                 xUdskriv:Bool=xUdskriv,
                 xJournalHer:Bool=xJournalHer,
                 xJournalOpdateret:Bool=xJournalOpdateret,
                 xOperation:Bool=xOperation,
                 xInformeret:Bool=xInformeret,
                 xReserveret:Bool=xReserveret,
                 xMedicinMorgen:Bool=xMedicinMorgen,
                 xMedicinEftermiddag:Bool=xMedicinEftermiddag,
                 xMedicinAften:Bool=xMedicinAften>.
    {^Stuegang[[xJournalHer and not xMedicinMorgen and not xMedicinEftermiddag and not xMedicinAften]]:
      3=>2:2<$journal<true,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2)>;
      //1=>2:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      rec $stuegang<xUrinering:Bool=xUrinering,
                    xAffoering:Bool=xAffoering,
                    xUdskriv:Bool=xUdskriv,
                    xJournalHer:Bool=xJournalHer,
                    xJournalOpdateret:Bool=xJournalOpdateret,
                    xOperation:Bool=xOperation,
                    xInformeret:Bool=xInformeret,
                    xReserveret:Bool=xReserveret,
                    xMedicinMorgen:Bool=xMedicinMorgen,
                    xMedicinEftermiddag:Bool=xMedicinEftermiddag,
                    xMedicinAften:Bool=xMedicinAften>.
        {#Operer:
          2=>1:1<Bool> as newOperation;
          2=>3:3<Bool> as pOperation [[(newOperation or not pOperation) and (not newOperation or pOperation)]]; // =newOperation
          2=>1:1<Bool> as newInformeret; // [[newOperation or not newInformeret]];
          2=>3:3<Bool> as pInformeret [[(newInformeret or not pInformeret) and (not newInformeret or pInformeret)]]; // =newInformeret
          $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,newOperation,newInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
         #Udskriv[[xUrinering and xAffoering]]:
          2=>1:1<Bool> as newUdskriv; // [[(xUrinering and xAffoering) or not newUdskriv]];
          2=>3:3<Bool> as pUdskriv [[(newUdskriv or not pUdskriv) and (not newUdskriv or pUdskriv)]]; // =newUdskriv
          $stuegang<xUrinering,xAffoering,newUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
         #Medicin:
          2=>1:1<String>; // Læge informerer om medicinering
          2=>3:3<String>; // Læge informerer om medicinering
          2=>1:1<Bool> as newMedicinMorgen;
          2=>3:3<Bool> as pMedicinMorgen [[(newMedicinMorgen or not pMedicinMorgen) and (not newMedicinMorgen or pMedicinMorgen)]]; // =newMedicinMorgen
          2=>1:1<Bool> as newMedicinEftermiddag;
          2=>3:3<Bool> as pMedicinEftermiddag [[(newMedicinEftermiddag or not pMedicinEftermiddag) and (not newMedicinEftermiddag or pMedicinEftermiddag)]]; // =newMedicinEftermiddag
          2=>1:1<Bool> as newMedicinAften;
          2=>3:3<Bool> as pMedicinAften [[(newMedicinAften or not pMedicinAften) and (not newMedicinAften or pMedicinAften)]]; // =newMedicinAften
          $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,newMedicinMorgen,newMedicinEftermiddag,newMedicinAften>,
         ^AfslutStuegang:
          2=>3:3<$journal<true,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2)>;
          $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>
        },
     #Dikter[[xJournalHer]]:
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,true,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^SendJournal[[xJournalHer and xJournalOpdateret]]:
      $treatment<xUrinering,xAffoering,xUdskriv,false,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^HentJournal[[not xJournalHer]]:
      $treatment<xUrinering,xAffoering,xUdskriv,true,false,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     #Flyt[[xJournalHer]]:
      // Nyt værelse skal noteres i Journalen
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynMorgen[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      3=>2:2<Bool> as pUrinering [[(newUrinering or not pUrinering) and (not newUrinering or pUrinering)]]; // =newUrinering (not done physically)
      1=>3:3<Bool> as newAffoering;
      3=>2:2<Bool> as pAffoering [[(newAffoering or not pAffoering) and (not newAffoering or pAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<newUrinering,newAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynEftermiddag[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      3=>2:2<Bool> as pUrinering [[(newUrinering or not pUrinering) and (not newUrinering or pUrinering)]]; // =newUrinering (not done physically)
      1=>3:3<Bool> as newAffoering;
      3=>2:2<Bool> as pAffoering [[(newAffoering or not pAffoering) and (not newAffoering or pAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<newUrinering,newAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynAften[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      3=>2:2<Bool> as pUrinering [[(newUrinering or not pUrinering) and (not newUrinering or pUrinering)]]; // =newUrinering (not done physically)
      1=>3:3<Bool> as newAffoering;
      3=>2:2<Bool> as pAffoering [[(newAffoering or not pAffoering) and (not newAffoering or pAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<newUrinering,newAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^MedicinMorgen[[xJournalHer and xMedicinMorgen]]: // Time Constraint?
      // Sygeplejer giver medicinen til patienten
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,false,xMedicinEftermiddag,xMedicinAften>,
     ^MedicinEftermiddag[[xJournalHer and xMedicinEftermiddag]]: // Time Constraint?
      // Sygeplejer giver medicinen til patienten
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,false,xMedicinAften>,
     ^MedicinAften[[xJournalHer and xMedicinAften]]: // Time Constraint?
      // Sygeplejer giver medicinen til patienten
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,false>,
      // reserver, operation
     ^Reserver[[xJournalHer and xOperation and not xReserveret]]:
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,true,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^Operer[[xJournalHer and xOperation and xInformeret and xReserveret]]:
      {^AfslutOperation:
        $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,false,false,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>
      },
     ^Udskriv[[xUdskriv and xJournalHer]]: Gend
    }
in
define $2114_stuegang xUrinering xAffoering
                      xUdskriv
                      xJournalHer xJournalOpdateret
                      xOperation xInformeret xReserveret
                      xMedicinMorgen xMedicinEftermiddag xMedicinAften =
  rec $stuegang<xUrinering:Bool=xUrinering,
                xAffoering:Bool=xAffoering,
                xUdskriv:Bool=xUdskriv,
                xJournalHer:Bool=xJournalHer,
                xJournalOpdateret:Bool=xJournalOpdateret,
                xOperation:Bool=xOperation,
                xInformeret:Bool=xInformeret,
                xReserveret:Bool=xReserveret,
                xMedicinMorgen:Bool=xMedicinMorgen,
                xMedicinEftermiddag:Bool=xMedicinEftermiddag,
                xMedicinAften:Bool=xMedicinAften>.
    {#Operer:
      2=>1:1<Bool> as newOperation;
      2=>3:3<Bool> as pOperation [[(newOperation or not pOperation) and (not newOperation or pOperation)]]; // =newOperation
      2=>1:1<Bool> as newInformeret; // [[newOperation or not newInformeret]];
      2=>3:3<Bool> as pInformeret [[(newInformeret or not pInformeret) and (not newInformeret or pInformeret)]]; // =newInformeret
      $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,newOperation,newInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     #Udskriv[[xUrinering and xAffoering]]:
      2=>1:1<Bool> as newUdskriv; // [[(xUrinering and xAffoering) or not newUdskriv]];
      2=>3:3<Bool> as pUdskriv [[(newUdskriv or not pUdskriv) and (not newUdskriv or pUdskriv)]]; // =newUdskriv
      $stuegang<xUrinering,xAffoering,newUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     #Medicin:
      2=>1:1<String>; // Læge informerer om medicinering
      2=>3:3<String>; // Læge informerer om medicinering
      2=>1:1<Bool> as newMedicinMorgen;
      2=>3:3<Bool> as pMedicinMorgen [[(newMedicinMorgen or not pMedicinMorgen) and (not newMedicinMorgen or pMedicinMorgen)]]; // =newMedicinMorgen
      2=>1:1<Bool> as newMedicinEftermiddag;
      2=>3:3<Bool> as pMedicinEftermiddag [[(newMedicinEftermiddag or not pMedicinEftermiddag) and (not newMedicinEftermiddag or pMedicinEftermiddag)]]; // =newMedicinEftermiddag
      2=>1:1<Bool> as newMedicinAften;
      2=>3:3<Bool> as pMedicinAften [[(newMedicinAften or not pMedicinAften) and (not newMedicinAften or pMedicinAften)]]; // =newMedicinAften
      $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,newMedicinMorgen,newMedicinEftermiddag,newMedicinAften>,
     ^AfslutStuegang:
      2=>3:3<$journal<true,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2)>;
      $2114<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>
    }
in // }}}
// Opret Kanal: behandling {{{
(nu behandling :
  1=>3:3<$journal<true,false,false,false,false,false,false,false,false>@(1of2)>;
  $2114<false,false,false,true,false,false,false,false,false,false,false>) // }}}
( // Laege service Implementation {{{
  def Laege() =
    link(3,behandling,s,2); // Forbind som Læge
    ( Laege()
    | def Behandling<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
                     xJournalHer:Bool,xJournalOpdateret:Bool,
                     xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                     xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
              (b:$2114<xUrinering,xAffoering,xUdskriv,
                       xJournalHer,xJournalOpdateret,
                       xOperation,xInformeret,xReserveret,
                       xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(2of3))=
        def Stuegang<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
                     xJournalOpdateret:Bool,
                     xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                     xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
                     (b:$2114_stuegang<xUrinering,xAffoering,xUdskriv,
                              true,xJournalOpdateret,
                              xOperation,xInformeret,xReserveret,
                              xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(2of3),
                      j:$journal<true,xUrinering,xAffoering,
                                 xOperation,xInformeret,xReserveret,
                                 xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2)) =
          // FIXME Update GUI
          guisync(3,b,2)
          {#Operer(operer:Bool=xOperation,
                   patientinformeret:Bool=false):
            b[1]<<operer;
            b[3]<<operer;
            b[1]<<patientinformeret;
            b[3]<<patientinformeret;
            j[1]<<^SkrivOperation;
            j[1]<<operer;
            j[1]<<^SkrivInformeret;
            j[1]<<patientinformeret;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,operer,patientinformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Udskriv[[xUrinering and xAffoering]](udskriv:Bool=xUdskriv):
            b[1]<<udskriv;
            b[3]<<udskriv;
            Stuegang<xUrinering,xAffoering,udskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Medicin(medicin:String="",
                    morgen:Bool=xMedicinMorgen,
                    eftermiddag:Bool=xMedicinEftermiddag,
                    aften:Bool=xMedicinAften):
            b[1]<<medicin;
            b[3]<<medicin;
            j[1]<<^SkrivMedicin;
            j[1]<<medicin;
            b[1]<<morgen;
            b[3]<<morgen;
            b[1]<<eftermiddag;
            b[3]<<eftermiddag;
            b[1]<<aften;
            b[3]<<aften;
            j[1]<<^SkrivMedicinering;
            j[1]<<morgen;
            j[1]<<eftermiddag;
            j[1]<<aften;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,morgen,eftermiddag,aften>(b,j),
           ^AfslutStuegang(kommentar:String=""):
            b[3]<<j;
            Behandling<xUrinering,xAffoering,xUdskriv,true,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
          }
        in
          guisync(3,b,2)
          {^Stuegang[[xJournalHer and not xMedicinMorgen and not xMedicinEftermiddag and not xMedicinAften]](kommentar:String=""):
            b[2]>>j;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Dikter[[xJournalHer]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,true,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^SendJournal[[xJournalHer and xJournalOpdateret]]():
            Behandling<xUrinering,xAffoering,xUdskriv,false,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^HentJournal[[not xJournalHer]]():
            Behandling<xUrinering,xAffoering,xUdskriv,true,false,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           #Flyt[[xJournalHer]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^TilsynMorgen[[xJournalHer]]():
            b[2]>>xUrinering;
            b[2]>>xAffoering;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^TilsynEftermiddag[[xJournalHer]]():
            b[2]>>xUrinering;
            b[2]>>xAffoering;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^TilsynAften[[xJournalHer]]():
            b[2]>>xUrinering;
            b[2]>>xAffoering;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^MedicinMorgen[[xJournalHer and xMedicinMorgen]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,false,xMedicinEftermiddag,xMedicinAften>(b),
           ^MedicinEftermiddag[[xJournalHer and xMedicinEftermiddag]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,false,xMedicinAften>(b),
           ^MedicinAften[[xJournalHer and xMedicinAften]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,false>(b),
           ^Reserver[[xJournalHer and xOperation and not xReserveret]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,true,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           ^Operer[[xJournalHer and xOperation and xInformeret and xReserveret]](kommentar:String=""):
            guisync(3,b,2)
            {^AfslutOperation(kommentar:String=kommentar):
              Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,false,false,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
            },
           ^Udskriv[[xUdskriv and xJournalHer]](): end
          }
      in
        Behandling<false,false,false,true,false,false,false,false,false,false,false>(s)
    )
  in Laege() | // }}}
( // Sygeplejer Service Implementation {{{
  def Sygeplejer() =
    link(3,behandling,s,3); // Forbind som Sygeplejer
    ( Sygeplejer()
    | def MedJournal<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool, // {{{
                     xJournalOpdateret:Bool,
                     xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                     xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
              (b:$2114<xUrinering,xAffoering,xUdskriv,
                       true,xJournalOpdateret,
                       xOperation,xInformeret,xReserveret,
                       xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(3of3),
               j:$journal<true,xUrinering,xAffoering,
                          xOperation,xInformeret,xReserveret,
                          xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2))=
      def Stuegang<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool, // {{{
                   xJournalOpdateret:Bool,
                   xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                   xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
                   (b:$2114_stuegang<xUrinering,xAffoering,xUdskriv,
                            true,xJournalOpdateret,
                            xOperation,xInformeret,xReserveret,
                            xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(3of3)) =
        // FIXME Update GUI
        guisync(3,b,3)
        {#Operer():
          b[3]>>operer;
          b[3]>>patientinformeret;
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,operer,patientinformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Udskriv[[xUrinering and xAffoering]]():
          b[3]>>udskriv;
          Stuegang<xUrinering,xAffoering,udskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Medicin():
          b[3]>>medicin;
          b[3]>>morgen;
          b[3]>>eftermiddag;
          b[3]>>aften;
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,morgen,eftermiddag,aften>(b),
         ^AfslutStuegang(kommentar:String=""):
          b[3]>>j;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j)
        } // }}}
      in
      def UdenJournal<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool, // {{{
                     xJournalOpdateret:Bool,
                     xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                     xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
              (b:$2114<xUrinering,xAffoering,xUdskriv,
                       false,xJournalOpdateret,
                       xOperation,xInformeret,xReserveret,
                       xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(3of3),
               s:$hent_journal<xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of2))=
        // FIXME: Update GUI
        guisync(3,b,3)
        {^HentJournal(kommentar:String=""):
          s[2]>>j;
          MedJournal<xUrinering,xAffoering,xUdskriv,false,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j)
        }
//        UdenJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j) // }}}
      in
        // FIXME: Update GUI
        guisync(3,b,3)
        {^Stuegang[[not xMedicinMorgen and not xMedicinEftermiddag and not xMedicinAften]](kommentar:String=""):
          b[2]<<j;
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Dikter():
          MedJournal<xUrinering,xAffoering,xUdskriv,true,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^SendJournal[[xJournalOpdateret]]():
          link(2,sekretaer,s,1);
          s[1]<<xUrinering;
          s[1]<<xAffoering;
          s[1]<<xOperation;
          s[1]<<xInformeret;
          s[1]<<xReserveret;
          s[1]<<xMedicinMorgen;
          s[1]<<xMedicinEftermiddag;
          s[1]<<xMedicinAften;
          s[1]<<j;
          UdenJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,s),
         #Flyt(vaerelse:String=""): //FIXME: Brug nuværende værelse som default
          j[1]<<^SkrivVaerelse;
          j[1]<<vaerelse;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^TilsynMorgen():
          b[3]>>xUrinering;
          b[2]<<xUrinering;
          b[3]>>xAffoering;
          b[2]<<xAffoering;
          j[1]<<^SkrivUrinering;
          j[1]<<xUrinering;
          j[1]<<^SkrivAffoering;
          j[1]<<xAffoering;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^TilsynEftermiddag():
          b[3]>>xUrinering;
          b[2]<<xUrinering;
          b[3]>>xAffoering;
          b[2]<<xAffoering;
          j[1]<<^SkrivUrinering;
          j[1]<<xUrinering;
          j[1]<<^SkrivAffoering;
          j[1]<<xAffoering;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^TilsynAften():
          b[3]>>xUrinering;
          b[2]<<xUrinering;
          b[3]>>xAffoering;
          b[2]<<xAffoering;
          j[1]<<^SkrivUrinering;
          j[1]<<xUrinering;
          j[1]<<^SkrivAffoering;
          j[1]<<xAffoering;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^MedicinMorgen[[xMedicinMorgen]](kommentar:String=""):
          j[1]<<^SkrivMedicinering;
          j[1]<<false;
          j[1]<<xMedicinEftermiddag;
          j[1]<<xMedicinAften;
         MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,false,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^MedicinEftermiddag[[xMedicinEftermiddag]](kommentar:String=""):
          j[1]<<^SkrivMedicinering;
          j[1]<<xMedicinMorgen;
          j[1]<<false;
          j[1]<<xMedicinAften;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,false,xMedicinAften>(b,j),
         ^MedicinAften[[xMedicinAften]](kommentar:String=""):
          j[1]<<^SkrivMedicinering;
          j[1]<<xMedicinMorgen;
          j[1]<<xMedicinEftermiddag;
          j[1]<<false;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,false>(b,j),
         ^Reserver[[xOperation and not xReserveret]](kommentar:String=""):
          j[1]<<^SkrivReserveret;
          j[1]<<true;
          MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,true,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
         ^Operer[[xOperation and xInformeret and xReserveret]](kommentar:String=""):
          guisync(3,b,3)
          {^AfslutOperation(kommentar:String=kommentar):
            j[1]<<^SkrivOperation;
            j[1]<<false;
            j[1]<<^SkrivInformeret;
            j[1]<<false;
            j[1]<<^SkrivReserveret;
            j[1]<<false;
            MedJournal<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,false,false,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j)
          },
         ^Udskriv[[xUdskriv]](kommentar:String=""):
           j[1]<<^SkrivIndskrevet;
           j[1]<<false;
           j[1]<<^Arkiver;
           end
        } // }}}
      in
        s[3]>>j; // Modtag journal
        MedJournal<false,false,false,false,false,false,false,false,false,false>(s,j)
    )
  in Sygeplejer() | // }}}
  // }}}
  // Skranke {{{
// Skranke Interface (reception) {{{
// Deltagere:
// 1: Patient
// 2: Receptionist
define $skranke =
  1=>2:2<String>; // Send Navn
  1=>2:2<String>; // Send CPR
  1=>2:2<String>; // Send Symptomer
  {^Indlaeg:
    2=>1:1<$2114<false,false,false,true,false,false,false,false,false,false,false>@(1of3)>;
    Gend
  }
in // }}}
//// Opret Kanal: skranke {{{
(nu skranke : $skranke) // }}}
( // Receptionist {{{
  def Receptionist() =
    link(2,skranke,s,2);
    ( Receptionist()
    | s[2]>>navn;
      s[2]>>cpr;
      s[2]>>symptomer;
      link(2,journal,j,1); // Opret Journal
      j[1]<<navn;
      j[1]<<cpr;
      guivalue(2,s,2,"Navn:",navn);
      guivalue(2,s,2,"CPR:",cpr);
      guivalue(2,s,2,"Symptomer:",symptomer);
      guisync(2,s,2)
      {^Indlaeg(vaerelse:String="Værelse12"): // Indlæs vaerelse fra brugerflade
        j[1]<<vaerelse;
        j[1]<<symptomer;
        link(3,behandling,b,1); // Opret Behandling
        b[3]<<j;
        s[1]<<b;
        end
      }
    )
  in Receptionist() | // }}}
  // }}}
( def Patient(navn: String,cpr: String,symptomer: String) = // {{{
    def Behandling<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
                   xJournalHer:Bool,xJournalOpdateret:Bool,
                   xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                   xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
            (b:$2114<xUrinering,xAffoering,xUdskriv,
                     xJournalHer,xJournalOpdateret,
                     xOperation,xInformeret,xReserveret,
                     xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of3))=
      def Stuegang<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
                   xJournalHer:Bool,xJournalOpdateret:Bool,
                   xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                   xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
                   (b:$2114_stuegang<xUrinering,xAffoering,xUdskriv,
                            xJournalHer,xJournalOpdateret,
                            xOperation,xInformeret,xReserveret,
                            xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(1of3)) =
        // FIXME: Update GUI
        guivalue(3,b,1,"Klar til udskrivning",if xUdskriv then "Ja" else "Nej");
        guivalue(3,b,1,"Medicin-Morgen",xMedicinMorgen);
        guivalue(3,b,1,"Medicin-Eftermiddag",xMedicinEftermiddag);
        guivalue(3,b,1,"Medicin-Aften",xMedicinAften);
        guisync(3,b,1)
        {#Operer():
          b[1]>>operer;
          b[1]>>patientinformeret;
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,operer,patientinformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Udskriv[[xUrinering and xAffoering]]():
          b[1]>>udskriv;
          Stuegang<xUrinering,xAffoering,udskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Medicin():
          b[1]>>medicin;
          guivalue(3,b,1,"Medicin",medicin);
          b[1]>>xMedicinMorgen;
          b[1]>>xMedicinEftermiddag;
          b[1]>>xMedicinAften;
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^AfslutStuegang():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
        }
      in
        // FIXME: Update GUI
        guisync(3,b,1)
        {^Stuegang[[xJournalHer and not xMedicinMorgen and not xMedicinEftermiddag and not xMedicinAften]](kommentar:String=""):
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Dikter[[xJournalHer]]():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,true,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^SendJournal[[xJournalHer and xJournalOpdateret]]():
          Behandling<xUrinering,xAffoering,xUdskriv,false,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^HentJournal[[not xJournalHer]]():
          Behandling<xUrinering,xAffoering,xUdskriv,true,false,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         #Flyt[[xJournalHer]]():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^TilsynMorgen[[xJournalHer]](urinering:Bool=xUrinering,affoering:Bool=xAffoering):
          b[3]<<urinering;
          b[3]<<affoering;
          Behandling<urinering,affoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^TilsynEftermiddag[[xJournalHer]](urinering:Bool=xUrinering,affoering:Bool=xAffoering):
          b[3]<<urinering;
          b[3]<<affoering;
          Behandling<urinering,affoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^TilsynAften[[xJournalHer]](urinering:Bool=xUrinering,affoering:Bool=xAffoering):
          b[3]<<urinering;
          b[3]<<affoering;
          Behandling<urinering,affoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^MedicinMorgen[[xJournalHer and xMedicinMorgen]]():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,false,xMedicinEftermiddag,xMedicinAften>(b),
         ^MedicinEftermiddag[[xJournalHer and xMedicinEftermiddag]]():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,false,xMedicinAften>(b),
         ^MedicinAften[[xJournalHer and xMedicinAften]]():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,false>(b),
         ^Reserver[[xJournalHer and xOperation and not xReserveret]]():
          Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,true,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
         ^Operer[[xJournalHer and xOperation and xInformeret and xReserveret]](kommentar:String=""):
          sync(3,b)
          {^AfslutOperation:
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,false,false,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
          },
         ^Udskriv[[xUdskriv and xJournalHer]](): end
        }
    in
      link(2,skranke,s,1); // Forbind som Patient
      s[2]<<navn;
      s[2]<<cpr;
      s[2]<<symptomer;
      sync(2,s)
      {^Indlaeg:
        s[1]>>b; // Modtag behandlings session (forbindelse til læge og sygeplejer)
        Behandling<false,false,false,true,false,false,false,false,false,false,false>(b)
      }
  in // }}}
// Opretter Patienter
Patient("Lasse Nielsen","170681....","Ondt i mavsen")
) ) ) ) ) )
