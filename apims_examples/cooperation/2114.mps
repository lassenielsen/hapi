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
    {^ReadName[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteName[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadCPR[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteCPR[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadRoom[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteRoom[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadVaeskeindtag[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteVaeskeindtag[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadEnergiindtag[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteEnergiindtag[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadSymptomer[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteSymptomer[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadAftale[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteAftale[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^ReadMedicin[[xIndskrevet]]:
      2=>1:2<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteMedicin[[xIndskrevet]]:
      1=>2:1<String>;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteUrinering[[xIndskrevet]]:
      1=>2:1<Bool> as xUrinering;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteAffoering[[xIndskrevet]]:
      1=>2:1<Bool> as xAffoering;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteOperation[[xIndskrevet]]:
      1=>2:1<Bool> as xOperation;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteInformeret[[xIndskrevet]]:
      1=>2:1<Bool> as xInformeret;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteReserveret[[xIndskrevet]]:
      1=>2:1<Bool> as xReserveret;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteMedicinering[[xIndskrevet]]:
      1=>2:1<Bool> as xMedicinMorgen;
      1=>2:1<Bool> as xMedicinEftermiddag;
      1=>2:1<Bool> as xMedicinAften;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^WriteIndskrevet:
      1=>2:1<Bool> as xIndskrevet;
      $actions<xIndskrevet,xUrinering,xAffoering,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^Arkiver[[not xIndskrevet]]: Gend
    }
in // }}}
// Opret Kanal: journal {{{
(nu journal :
  1=>2:1<String>; // Set Name 
  1=>2:1<String>; // Set CPR
  1=>2:1<String>; // Set Room
  1=>2:1<String>; // Set Symptoms
  $journal<true,false,false,false,false,false,false,false,false>) // }}}
( // Journal Service Implementation {{{
  def Journal() =
    link(2,journal,s,2);
    ( Journal()
    | s[1]>>name;
      s[1]>>cpr;
      s[1]>>room;
      s[1]>>symptomer;
      def State<theIndskrevet:Bool, theUrinering:Bool, theAffoering:Bool,
                theOperation:Bool, theInformeret:Bool, theReserveret:Bool,
                theMedicinMorgen:Bool,theMedicinEftermiddag:Bool,
                theMedicinAften:Bool>
               (s:$journal<theIndskrevet,theUrinering,theAffoering,
                           theOperation,theInformeret,theReserveret,
                           theMedicinMorgen,theMedicinEftermiddag,
                           theMedicinAften>@(2of2),
                theName:String, theCPR:String, theRoom:String,
                theVaeskeindtag:String, theEnergiindtag:String,
                theSymptomer:String, theAftale:String, theMedicin:String) =
        s[1]>>
        {^ReadName:
          s[2]<<theName;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^WriteName:
          s[1]>>theName;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^ReadCPR:
          s[2]<<theCPR;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^WriteCPR:
          s[1]>>theCPR;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^ReadRoom:
          s[2]<<theCPR;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^WriteRoom:
          s[1]>>theCPR;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^ReadVaeskeindtag:
          s[2]<<theVaeskeindtag;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^WriteVaeskeindtag:
          s[1]>>theVaeskeindtag;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^ReadEnergiindtag:
          s[2]<<theEnergiindtag;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^WriteEnergiindtag:
          s[1]>>theEnergiindtag;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^ReadSymptomer:
          s[2]<<theSymptomer;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^WriteSymptomer:
          s[1]>>theSymptomer;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^ReadAftale:
          s[2]<<theAftale;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteAftale:
          s[1]>>theAftale;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^ReadMedicin:
          s[2]<<theMedicin;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theMedicin,theMedicin,theMedicin),
         ^WriteMedicin:
          s[1]>>theMedicin;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theMedicin,theMedicin,theMedicin),
         ^WriteUrinering:
          s[1]>>theUrinering;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteAffoering:
          s[1]>>theAffoering;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteOperation:
          s[1]>>theOperation;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteInformeret:
          s[1]>>theInformeret;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteReserveret:
          s[1]>>theReserveret;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteMedicinering:
          s[1]>>theMedicinMorgen;
          s[1]>>theMedicinEftermiddag;
          s[1]>>theMedicinAften;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theAftale,theAftale,theMedicin),
         ^WriteIndskrevet:
          s[1]>>theIndskrevet;
          State<theIndskrevet,theUrinering,theAffoering,theOperation,theInformeret,theReserveret,theMedicinMorgen,theMedicinEftermiddag,theMedicinAften>
               (s,theName,theCPR,theRoom,theVaeskeindtag,theEnergiindtag,theSymptomer,theAftale,theMedicin),
         ^Arkiver: end
        }
      in State<true,false,false,false,false,false,false,false,false>(s,name,cpr,room,"(unknown)","(unknown)",symptomer,"Ingen","Ingen")
    )
  in Journal()
| // }}}
// Behandling {{{
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
          2=>3:3<Bool> as theOperation [[(newOperation or not theOperation) and (not newOperation or theOperation)]]; // =newOperation
          2=>1:1<Bool> as newInformeret; // [[newOperation or not newInformeret]];
          2=>3:3<Bool> as theInformeret [[(newInformeret or not theInformeret) and (not newInformeret or theInformeret)]]; // =newInformeret
          $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,newOperation,newInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
         #Udskriv[[xUrinering and xAffoering]]:
          2=>1:1<Bool> as newUdskriv; // [[(xUrinering and xAffoering) or not newUdskriv]];
          2=>3:3<Bool> as theUdskriv [[(newUdskriv or not theUdskriv) and (not newUdskriv or theUdskriv)]]; // =newUdskriv
          $stuegang<xUrinering,xAffoering,newUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
         #Medicin:
          2=>1:1<String>; // Læge informerer om medicinering
          2=>3:3<String>; // Læge informerer om medicinering
          2=>1:1<Bool> as newMedicinMorgen;
          2=>3:3<Bool> as theMedicinMorgen [[(newMedicinMorgen or not theMedicinMorgen) and (not newMedicinMorgen or theMedicinMorgen)]]; // =newMedicinMorgen
          2=>1:1<Bool> as newMedicinEftermiddag;
          2=>3:3<Bool> as theMedicinEftermiddag [[(newMedicinEftermiddag or not theMedicinEftermiddag) and (not newMedicinEftermiddag or theMedicinEftermiddag)]]; // =newMedicinEftermiddag
          2=>1:1<Bool> as newMedicinAften;
          2=>3:3<Bool> as theMedicinAften [[(newMedicinAften or not theMedicinAften) and (not newMedicinAften or theMedicinAften)]]; // =newMedicinAften
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
      3=>2:2<Bool> as theUrinering [[(newUrinering or not theUrinering) and (not newUrinering or theUrinering)]]; // =newUrinering (not done physically)
      1=>3:3<Bool> as newAffoering;
      3=>2:2<Bool> as theAffoering [[(newAffoering or not theAffoering) and (not newAffoering or theAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<newUrinering,newAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynEftermiddag[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      3=>2:2<Bool> as theUrinering [[(newUrinering or not theUrinering) and (not newUrinering or theUrinering)]]; // =newUrinering (not done physically)
      1=>3:3<Bool> as newAffoering;
      3=>2:2<Bool> as theAffoering [[(newAffoering or not theAffoering) and (not newAffoering or theAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<newUrinering,newAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynAften[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      3=>2:2<Bool> as theUrinering [[(newUrinering or not theUrinering) and (not newUrinering or theUrinering)]]; // =newUrinering (not done physically)
      1=>3:3<Bool> as newAffoering;
      3=>2:2<Bool> as theAffoering [[(newAffoering or not theAffoering) and (not newAffoering or theAffoering)]]; // =newAffoering (not done physically)
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
      2=>3:3<Bool> as theOperation [[(newOperation or not theOperation) and (not newOperation or theOperation)]]; // =newOperation
      2=>1:1<Bool> as newInformeret; // [[newOperation or not newInformeret]];
      2=>3:3<Bool> as theInformeret [[(newInformeret or not theInformeret) and (not newInformeret or theInformeret)]]; // =newInformeret
      $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,newOperation,newInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     #Udskriv[[xUrinering and xAffoering]]:
      2=>1:1<Bool> as newUdskriv; // [[(xUrinering and xAffoering) or not newUdskriv]];
      2=>3:3<Bool> as theUdskriv [[(newUdskriv or not theUdskriv) and (not newUdskriv or theUdskriv)]]; // =newUdskriv
      $stuegang<xUrinering,xAffoering,newUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     #Medicin:
      2=>1:1<String>; // Læge informerer om medicinering
      2=>3:3<String>; // Læge informerer om medicinering
      2=>1:1<Bool> as newMedicinMorgen;
      2=>3:3<Bool> as theMedicinMorgen [[(newMedicinMorgen or not theMedicinMorgen) and (not newMedicinMorgen or theMedicinMorgen)]]; // =newMedicinMorgen
      2=>1:1<Bool> as newMedicinEftermiddag;
      2=>3:3<Bool> as theMedicinEftermiddag [[(newMedicinEftermiddag or not theMedicinEftermiddag) and (not newMedicinEftermiddag or theMedicinEftermiddag)]]; // =newMedicinEftermiddag
      2=>1:1<Bool> as newMedicinAften;
      2=>3:3<Bool> as theMedicinAften [[(newMedicinAften or not theMedicinAften) and (not newMedicinAften or theMedicinAften)]]; // =newMedicinAften
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
( def Laege() = // {{{
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
                     xJournalHer:Bool,xJournalOpdateret:Bool,
                     xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                     xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
                     (b:$2114_stuegang<xUrinering,xAffoering,xUdskriv,
                              xJournalHer,xJournalOpdateret,
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
            j[1]<<^WriteOperation;
            j[1]<<operer;
            j[1]<<^WriteInformeret;
            j[1]<<patientinformeret;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,operer,patientinformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Udskriv[[xUrinering and xAffoering]](udskriv:Bool=xUdskriv):
            b[1]<<udskriv;
            b[3]<<udskriv;
            Stuegang<xUrinering,xAffoering,udskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Medicin(medicin:String="",
                    morgen:Bool=xMedicinMorgen,
                    eftermiddag:Bool=xMedicinEftermiddag,
                    aften:Bool=xMedicinAften):
            b[1]<<medicin;
            b[3]<<medicin;
            j[1]<<^WriteMedicin;
            j[1]<<medicin;
            b[1]<<morgen;
            b[3]<<morgen;
            b[1]<<eftermiddag;
            b[3]<<eftermiddag;
            b[1]<<aften;
            b[3]<<aften;
            j[1]<<^WriteMedicinering;
            j[1]<<morgen;
            j[1]<<eftermiddag;
            j[1]<<aften;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,morgen,eftermiddag,aften>(b,j),
           ^AfslutStuegang(kommentar:String=""):
            b[3]<<j;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
          }
        in
          guisync(3,b,2)
          {^Stuegang[[xJournalHer and not xMedicinMorgen and not xMedicinEftermiddag and not xMedicinAften]](kommentar:String=""):
            b[2]>>j;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
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
( def Sygeplejer() = // {{{
    link(3,behandling,s,3); // Forbind som Sygeplejer
    ( Sygeplejer()
    | def Behandling<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
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
        def Stuegang<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
                     xJournalOpdateret:Bool,
                     xOperation:Bool,xInformeret:Bool,xReserveret:Bool,
                     xMedicinMorgen:Bool,xMedicinEftermiddag:Bool,xMedicinAften:Bool>
                     (b:$2114_stuegang<xUrinering,xAffoering,xUdskriv,
                              true,xJournalOpdateret,
                              xOperation,xInformeret,xReserveret,
                              xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(3of3)) =
          // FIXME Update GUI
          guisync(3,b,2)
          {#Operer():
            b[3]>>operer;
            b[3]>>patientinformeret;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,operer,patientinformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Udskriv[[xUrinering and xAffoering]]():
            b[3]>>udskriv;
            Stuegang<xUrinering,xAffoering,udskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Medicin():
            b[3]>>medicin;
            b[3]>>morgen;
            b[3]>>eftermiddag;
            b[3]>>aften;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,morgen,eftermiddag,aften>(b,j),
           ^AfslutStuegang(kommentar:String=""):
            b[3]>>j;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j)
          }
        in
          guisync(3,b,3)
          {^Stuegang[[not xMedicinMorgen and not xMedicinEftermiddag and not xMedicinAften]](kommentar:String=""):
            b[2]<<j;
            Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           #Dikter[[xJournalHer]]():
            Behandling<xUrinering,xAffoering,xUdskriv,true,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^SendJournal[[xJournalHer and xJournalOpdateret]]():
              // FIXME: Send journal til sekretaer
            BehandlingUdenJournal<xUrinering,xAffoering,xUdskriv,false,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b),
           #Flyt[[xJournalHer]](vaerelse:String=""): //FIXME: Brug nuværende værelse som default
            j[1]<<^WriteRoom;
            j[1]<<vaerelse;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^TilsynMorgen():
            b[3]>>xUrinering;
            b[2]<<xUrinering;
            b[3]>>xAffoering;
            b[2]<<xAffoering;
            j[1]<<^WriteUrinering;
            j[1]<<xUrinering;
            j[1]<<^WriteAffoering;
            j[1]<<xAffoering;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^TilsynEftermiddag():
            b[3]>>xUrinering;
            b[2]<<xUrinering;
            b[3]>>xAffoering;
            b[2]<<xAffoering;
            j[1]<<^WriteUrinering;
            j[1]<<xUrinering;
            j[1]<<^WriteAffoering;
            j[1]<<xAffoering;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^TilsynAften():
            b[3]>>xUrinering;
            b[2]<<xUrinering;
            b[3]>>xAffoering;
            b[2]<<xAffoering;
            j[1]<<^WriteUrinering;
            j[1]<<xUrinering;
            j[1]<<^WriteAffoering;
            j[1]<<xAffoering;
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^MedicinMorgen[[xMedicinMorgen]](kommentar:String=""):
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,false,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^MedicinEftermiddag[[xMedicinEftermiddag]](kommentar:String=""):
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,false,xMedicinAften>(b,j),
           ^MedicinAften[[xMedicinAften]]():
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,false>(b,j),
           ^Reserver[[xOperation and not xReserveret]](kommentar:String=""):
            Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,xOperation,xInformeret,true,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j),
           ^Operer[[xJournalHer and xOperation and xInformeret and xReserveret]](kommentar:String=""):
            guisync(3,b,2)
            {^AfslutOperation(kommentar:String=kommentar):
              Behandling<xUrinering,xAffoering,xUdskriv,xJournalOpdateret,false,false,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b,j)
            },
           ^Udskriv[[xUdskriv and xJournalHer]]():
             j[1]<<^WriteIndskrevet;
             j[1]<<false;
             j[1]<<^Arkiver;
             end
          }
      in
        s[3]>>j; // Modtag journal
        Behandling<false,false,false,true,false,false,false,false,false,false,false>(s,j)
    )
  in Sygeplejer() | // }}}
// Skranke (reception) Interface {{{
// Praticipants:
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
// Opret Kanal: skranke {{{
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
  in Patient("Lasse Nielsen","170681....","Ondt i mavsen") | // }}}
end
) ) ) ) )
