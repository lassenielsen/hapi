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
        {^Operer:
          2=>1:1<Bool> as newOperation;
          2=>3:3<Bool> as theOperation [[(newOperation or not theOperation) and (not newOperation or theOperation)]]; // =newOperation
          2=>1:1<Bool> as newInformeret; // [[newOperation or not newInformeret]];
          2=>3:3<Bool> as theInformeret [[(newInformeret or not theInformeret) and (not newInformeret or theInformeret)]]; // =newInformeret
          $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,newOperation,newInformeret,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
         ^Udskriv:
          2=>1:1<Bool> as newUdskriv; // [[(xUrinering and xAffoering) or not newUdskriv]];
          2=>3:3<Bool> as theUdskriv [[(newUdskriv or not theUdskriv) and (not newUdskriv or theUdskriv)]]; // =newUdskriv
          $stuegang<xUrinering,xAffoering,newUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
         ^Medicin:
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
      $treatment<xUrinering,xAffoering,xUdskriv,true,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     #Flyt[[xJournalHer]]:
      // Nyt værelse skal noteres i Journalen
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynMorgen[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      2=>3:3<Bool> as theUrinering [[(newUrinering or not theUrinering) and (not newUrinering or theUrinering)]]; // =newUrinering (not done physically)
      1=>3:1<Bool> as newAffoering;
      2=>3:3<Bool> as theAffoering [[(newAffoering or not theAffoering) and (not newAffoering or theAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynEftermiddag[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      2=>3:3<Bool> as theUrinering [[(newUrinering or not theUrinering) and (not newUrinering or theUrinering)]]; // =newUrinering (not done physically)
      1=>3:1<Bool> as newAffoering;
      2=>3:3<Bool> as theAffoering [[(newAffoering or not theAffoering) and (not newAffoering or theAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^TilsynAften[[xJournalHer]]: // Time Constraint?
      //1=>3:4<String>; // Patient forklarer symptomer
      // Symptomerne skal tilføjes journalen
      1=>3:3<Bool> as newUrinering;
      2=>3:3<Bool> as theUrinering [[(newUrinering or not theUrinering) and (not newUrinering or theUrinering)]]; // =newUrinering (not done physically)
      1=>3:1<Bool> as newAffoering;
      2=>3:3<Bool> as theAffoering [[(newAffoering or not theAffoering) and (not newAffoering or theAffoering)]]; // =newAffoering (not done physically)
      //1=>3:3<String>; // Patient forklarer væskeindtag
      //1=>3:3<String>; // Patient forklarer energiindtag
      // Væske- og energi-indtag noteres i journalen
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
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
     ^Operer[[xJournalHer and xOperation and xReserveret]]:
      $treatment<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,false,false,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
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
    {^Operer:
      2=>1:1<Bool> as newOperation;
      2=>3:3<Bool> as theOperation [[(newOperation or not theOperation) and (not newOperation or theOperation)]]; // =newOperation
      2=>1:1<Bool> as newInformeret; // [[newOperation or not newInformeret]];
      2=>3:3<Bool> as theInformeret [[(newInformeret or not theInformeret) and (not newInformeret or theInformeret)]]; // =newInformeret
      $stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,newOperation,newInformeret,false,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^Udskriv:
      2=>1:1<Bool> as newUdskriv; // [[(xUrinering and xAffoering) or not newUdskriv]];
      2=>3:3<Bool> as theUdskriv [[(newUdskriv or not theUdskriv) and (not newUdskriv or theUdskriv)]]; // =newUdskriv
      $stuegang<xUrinering,xAffoering,newUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>,
     ^Medicin:
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
( def Doctor() = // {{{
    link(3,behandling,s,2); // Forbind som Læge
    ( Doctor()
    | def State<xUrinering:Bool,xAffoering:Bool,xUdskriv:Bool,
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
                     (b:$2114<xUrinering,xAffoering,xUdskriv,
                              xJournalHer,xJournalOpdateret,
                              xOperation,xInformeret,xReserveret,
                              xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>@(2of3)) =
          Stuegang<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
        in
          guisync(3,s,2)
          {^Stuegang()
          State<xUrinering,xAffoering,xUdskriv,xJournalHer,xJournalOpdateret,xOperation,xInformeret,xReserveret,xMedicinMorgen,xMedicinEftermiddag,xMedicinAften>(b)
      in
        State<false,false,false,true,false,false,false,false,false,false,false>(s)
    )
  in Doctor() | // }}}
// Skranke (reception) Interface {{{
// Praticipants:
// 1: Patient
// 2: Receptionist
define $skranke =
  1=>2:2<String>; // Send Navn
  1=>2:2<String>; // Send CPR
  1=>2:2<String>; // Send Symptomer
  2=>1:1<$2114<false,false,false,true,false,false,false,false,false,false,false>@(1of3)>;
  Gend
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
      (nu gui: {^Opret: Gend}) // Opret brugerflade type
      link(1,gui,g,1); // Opret brugerflade
      guivalue(1,g,1,"Navn:",navn);
      guivalue(1,g,1,"CPR:",cpr);
      guivalue(1,g,1,"Symptomer:",symptomer);
      guisync(1,g,1)
      {^Opret(vaerelse:String="Værelse12"): // Indlæs vaerelse fra brugerflade
        j[1]<<vaerelse;
        j[1]<<symptomer;
        link(3,behandling,b,1); // Opret Behandling
        b[3]<<j;
        s[1]<<b;
        end
      }
    )
  in Receptionist() | // }}}
end
) ) )
