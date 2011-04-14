// DRUG: CHANNEL, TYPE AND IMPLEMENTATION {{{
// GLOBAL TYPE: $drug // {{{
// This is the Interface of a Drug
// Users:
//  1: The drug itself
//  2: User/Owner
define $drug_atadminister =  // Behavior when sent from Authorised person to Nurse
  2=>1:1
  {^Administer:              // The drug is administered to the patient
    Gend
  }
in define $drug_atcheck2 =   // Behavior when sent crom CP to Nurse via porter
  rec $atcheck2.
  2=>1:1
  {^Check:                   // Check Patient Information
    1=>2:2<String>;          // Read Content description
    1=>2:2<String>;          // Read Patient Information
    2=>1:1                   // Verify or Reject
    {^Reject:                // the content does not match prescription
      $atcheck2,             // The drug is destroyed, and the preparation must be restarted
     ^Verify:                // The content matches the prescription
      2=>1:1
      {^Check:               // Check Patient Information
        1=>2:2<String>;      // Read Content description
        1=>2:2<String>;      // Read Patient Information
        2=>1:1               // Verify or Reject
        {^Reject:            // the content does not match prescription
          $atcheck2,         // The drug is destroyed, and the preparation must be restarted
         ^Verify:            // The content matches the prescription
          $drug_atadminister
        }
      }
    },
   ^Destroy:
    Gend
  }
in define $drug_ataccept1 =  // Behavior when sent from PA to CP
  2=>1:1                     // Verify or Reject
  {^Destroy:                 // the content does not match prescription
    Gend,                    // The drug is destroyed, and the preparation must be restarted
   ^Verify:                  // The content matches the prescription
    $drug_atcheck2
  }
in define $drug_atcheck1 =   // Behavior when sent from PA to CP
  2=>1:1
  {^Check:                   // Check Patient Information
    1=>2:2<String>;          // Read content description
    1=>2:2<String>;          // Read Patient Information
    $drug_ataccept1
  }
in define $drug =            // Full behavior of a drug
  2=>1:1
  {^Prepare:
    2=>1:1<String>;          // Attach Content Information
    2=>1:1<String>;          // Attach Patient Information
    $drug_atcheck1
  }
// Used positions: // {{{
in define $drug_ataccept3 =  // Behavior when sent from PA to CP
  2=>1:1                     // Verify or Reject
  {^Reject:                  // the content does not match prescription
    $drug_atcheck2,          // The drug is destroyed, and the preparation must be restarted
   ^Verify:                  // The content matches the prescription
    $drug_atadminister
  }
in define $drug_atcheck3 =   // Behavior when sent from Nurse to Authorised person
  2=>1:1
  {^Check:                   // Check Patient Information
    1=>2:2<String>;          // Read content description
    1=>2:2<String>;          // Read Patient Information
    $drug_ataccept3
  }
in define $drug_ataccept2 =
  2=>1:1                     // Verify or Reject
  {^Reject:                  // the content does not match prescription
    $drug_atcheck2,          // The drug is destroyed, and the preparation must be restarted
   ^Verify:                  // The content matches the prescription
    $drug_check3
  } // }}}
in // }}}
(nu drug: $drug)
// This is the Drug Service Implementation
def Drug() = // {{{
  link(2,drug,s,1);
  guivalue(2,s,1,"User","Drug");
  ( Drug()
  | s[1]>>
    {^Prepare:
      s[1]>>content;
      s[1]>>patient;
      s[1]>>
      {^Check:
        s[2]<<content;
        s[2]<<patient;
        s[1]>>
        {^Destroy: end,
         ^Verify:
          def Drug_Check2(s: $drug_atcheck2@(1of2)) =
            s[1]>>
            {^Check:
              s[2]<<content;
              s[2]<<patient;
              s[1]>>
              {^Reject: Drug_Check2(s),
               ^Verify:
                s[1]>>
                {^Check:
                  s[2]<<content;
                  s[2]<<patient;
                  s[1]>>
                  {^Reject: Drug_Check2(s),
                   ^Verify:
                    s[1]>>
                    {^Administer: end
                    }
                  }
                }
              },
             ^Destroy: end
            }
          in Drug_Check2(s)
        }
      }
    }
  )
in (Drug() | // }}}
// }}}
// FLOWCHART: CHANNEL, TYPE AND IMPLEMENTATION {{{
// GLOBAL TYPE: $flowchart {{{
// This is the Interface of a Flowchart
// Users:
//  1: The Flowchart Object
//  2: User/Owner
define $flowchart_atsign =
  2=>1:1
  {^Sign:                            // The resonsible nurse signs
    Gend
  }
in define $flowchart_atcheck3 =
  rec $atcheck3.
  2=>1:1
  {^Check:                           // BEGIN: Responsible Nurse Check
    1=>2:2<String>;                  // Read Patient Info
    1=>2:2<String>;                  // Read Dosis
    2=>1:1
    {^Reject:                        // The prescription and prepared dosis do not match
      $atcheck3,
     ^Accept:                        // The prescription and prepared dosis match
      2=>1:1
      {^Check:                       // BEGIN: Authorized Person Check
        1=>2:2<String>;              // Read Patient Info
        1=>2:2<String>;              // Read Dosis
        2=>1:1
        {^Reject:                    // The prescription and prepared dosis do not match
          $atcheck3,                 // Put drug back, reverification is necessary
         ^Accept:                    // The prescription and prepared dosis match
          $flowchart_atsign
        }
      }
    }
  }
in define $flowchart_atcheck4 =
  2=>1:1
  {^Check:                           // BEGIN: Authorized Person Check
    1=>2:2<String>;                  // Read Patient Info
    1=>2:2<String>;                  // Read Dosis
    2=>1:1
    {^Reject:                        // The prescription and prepared dosis do not match
      $flowchart_atcheck3,           // Put drug back, reverification is necessary
     ^Accept:                        // The prescription and prepared dosis match
      $flowchart_atsign
    }
  }
// GLOBAL TYPE: $oncology_administer // {{{
// The administering of a drug
// Users:
//  1: The Patient
//  2: The Nurse
//  3: The Authorised Person
in define $oncology_administer =
  1=>2:2<String>;  // Patient gives info to nurse
  {^Reject:
    Gend,
   #Verify:
    1=>3:3<String>;                     // Patient gives details
    2=>3:4<$flowchart_atcheck4@(2of2)>; // Transfer flowchart to verify
    2=>3:4<$drug_atcheck3@(2of2)>;      // Transfer drug to verify
    {^Reject:
      3=>2:2<$flowchart_atcheck3@(2of2)>;
      3=>2:2<$drug_atcheck2@(2of2)>;// Transfer drug to verify
      Gend,
     #Verify:
      3=>2:2<$flowchart_atsign@(2of2)>;
      3=>2:2<$drug_atadminister@(2of2)>;// Transfer drug to verify
      {^Administer:
        Gend
      }
    }
  }// }}}
in define $flowchart_attonurse =
  1=>2:2< <$oncology_administer> >;  // The Nurse reads the contact info
  $flowchart_atcheck3
in define $flowchart_atcheck2 =
  rec $atcheck2.
  2=>1:1
  {^CheckDrug:                        // BEGIN: CP check PA work
    1=>2:2<String>;                   // Read Dosis
    1=>2:2<String>;                   // Read Patient
    2=>1:1
    {^RejectDrug:                     // The Patient Info did not match
      $atcheck2,                      // Try again when new drug is ready
     ^AcceptDrug:                     // The produced drug is accepted
      2=>1:1
      {^Sign:                         // The Controlling Pharmacist signs
        $flowchart_attonurse
      }
    }
  }
in define $flowchart_ataccept2 =
  2=>1:1
  {^RejectDrug:                       // The Patient Info did not match
    $flowchart_atcheck2,              // Try again when new drug is ready
   ^AcceptDrug:                       // The produced drug is accepted
    2=>1:1
    {^Sign:                           // The Controlling Pharmacist signs
      $flowchart_attonurse
    }
  }
in define $flowchart_atcalc =
  rec $atcalc.
  2=>1:1
  {^CalcDosis:
    2=>1:1<String>;                             // Write Dosis
    2=>1:1
    {^Sign:                                     // The Doctor signs
      2=>1:1
      {^Check:                                  // BEGIN: CP checks Prescription and drug
        1=>2:2<String>;                         // Read Patient Info
        1=>2:2<Int>;                            // Read Patient Height
        1=>2:2<Int>;                            // Read Patient Weight
        1=>2:2<String>;                         // Read Patient History
        1=>2:2<String>;                         // Read Latest Lab Results
        1=>2:2<String>;                         // Read Dosis
        rec $ataccept1.
        2=>1:1
        {^RejectCalc:
          $atcalc,
         ^AcceptCalc:
          rec $ataccept2.
          2=>1:1
          {^RejectDrug:                         // Calculations are wrong
            $ataccept2,                         // The calculations must be redone
           ^AcceptDrug:
            2=>1:1
            {^Sign:                             // The Controlling Pharmacist signs
              $flowchart_attonurse
            }
          },
         ^RejectDrug:
          $ataccept1,
         ^AcceptDrug:
          2=>1:1
          {^RejectCalc:                         // Calculations are wrong
            $atcalc,                            // The calculations must be redone
           ^AcceptCalc:
            2=>1:1
            {^Sign:                             // The Controlling Pharmacist signs
              $flowchart_attonurse
            }
          }
        }
      }
    }
  }
in define $flowchart_ataccept1no2 =
  2=>1:1
  {^RejectCalc:                                 // Calculations are wrong
    $flowchart_atcalc,                          // The calculations must be redone
   ^AcceptCalc:
    2=>1:1
    {^Sign:                                     // The Controlling Pharmacist signs
      $flowchart_attonurse
    }
  }
in define $flowchart_ataccept2no1 =
  rec $ataccept2.
  2=>1:1
  {^RejectDrug:                                 // Calculations are wrong
    $ataccept2,                                 // The calculations must be redone
   ^AcceptDrug:
    2=>1:1
    {^Sign:                                     // The Controlling Pharmacist signs
      $flowchart_attonurse
    }
  }
in define $flowchart_ataccept1 =
  rec $ataccept1.
  2=>1:1
  {^RejectCalc:
    $flowchart_atcalc,
   ^AcceptCalc:
    $flowchart_ataccept2no1,
   ^RejectDrug:
    $ataccept1,
   ^AcceptDrug:
    $flowchart_ataccept1no2
  }
in define $flowchart_atcheck1 =
  2=>1:1
  {^Check:
    1=>2:2<String>;                             // Read Patient Info
    1=>2:2<Int>;                                // Read Patient Height
    1=>2:2<Int>;                                // Read Patient Weight
    1=>2:2<String>;                             // Read Patient History
    1=>2:2<String>;                             // Read Latest Lab Results
    1=>2:2<String>;                             // Read Dosis
    $flowchart_ataccept1
  }
in define $flowchart =
  2=>1:1
  {^RegisterPatient:
    2=>1:1<String>;                             // Write Patient Info
    2=>1:1<Int>;                                // Write Patient Height
    2=>1:1<Int>;                                // Write Patient Weight
    2=>1:1<String>;                             // Write Patient History
    2=>1:1<String>;                             // Write Latest Lab results
    2=>1:1< <$oncology_administer> >;           // Write contact info
    $flowchart_atcalc
  }
in // }}}
(nu flowchart: $flowchart)
// This is the Flowchart Service Implementation
def Flowchart() = // {{{
  link(2,flowchart,s,1);
  guivalue(2,s,1,"User","Flowchart");
  ( Flowchart()
  | s[1]>>
    {^RegisterPatient:
      s[1]>>patient;
      s[1]>>height;
      s[1]>>weight;
      s[1]>>history;
      s[1]>>labresult;
      s[1]>>channel;
      def AtCalc(s:$flowchart_atcalc@(1of2)) =
      def AtAdminister(patient:String, // {{{
                       dosis:String,
                       s:$flowchart_atcheck3@(1of2)) =
        s[1]>>
        {^Check:
          s[2]<<patient;
          s[2]<<dosis;
          s[1]>>
          {^Reject:
            AtAdminister(patient,dosis,s),
           ^Accept:
            s[1]>>
            {^Check:
              s[2]<<patient;
              s[2]<<dosis;
              s[1]>>
              {^Reject:
                AtAdminister(patient,dosis,s),
               ^Accept:
                s[1]>>
                {^Sign:
                  end
                }
              }
            }
          }
        }
      in // }}}
      def AtAccept1no2(patient:String, // {{{
                       dosis:String,
                       s:$flowchart_ataccept1no2@(1of2)) =
        s[1]>>
        {^RejectCalc:
          AtCalc(s),
         ^AcceptCalc:
          s[1]>>
          {^Sign:
            s[2]<<channel;
            AtAdminister(patient,dosis,s)
          }
        }
      in // }}}
      def AtAccept2no1(patient:String, // {{{
                       dosis:String,
                       s:$flowchart_ataccept2no1@(1of2)) =
        s[1]>>
        {^RejectDrug:
          AtAccept2no1(patient,dosis,s),
         ^AcceptDrug:
          s[1]>>
          {^Sign:
            s[2]<<channel;
            AtAdminister(patient,dosis,s)
          }
        }
      in // }}}
      def AtAccept1(patient:String, // {{{
                    dosis:String,
                    s:$flowchart_ataccept1@(1of2)) =
        s[1]>>
        {^RejectCalc:
          AtCalc(s),
         ^AcceptCalc:
          AtAccept2no1(patient,dosis,s),
         ^RejectDrug:
          AtAccept1(patient,dosis,s),
         ^AcceptDrug:
          AtAccept1no2(patient,dosis,s)
        }
      in // }}}
        s[1]>>
        {^CalcDosis:
          s[1]>>dosis;
          s[1]>>
          {^Sign:
            s[1]>>
            {^Check:
              s[2]<<patient;
              s[2]<<height;
              s[2]<<weight;
              s[2]<<history;
              s[2]<<labresult;
              s[2]<<dosis;
              AtAccept1(patient,dosis,s)
            }
          }
        }
      in AtCalc(s)
    }
  )
in (Flowchart() | // }}}
// }}}
// PORTERS AND WARD INTERFACES {{{
// GLOBAL TYPE: $pharmacy_reception // {{{
// The reception of a flowchart at the pharmacy
// Users:
//  1: The Porter
//  2: The Controlling Pharmacist
define $pharmacy_reception =
  1=>2:2<$flowchart_atcheck1@(2of2)>;                  // Porter delivers flowchart to CP
  Gend
in // }}}
(nu drug_order: $pharmacy_reception)
(nu pharmacy_reception: $pharmacy_reception)
def Porter_Order() = // {{{
  link(2,drug_order,s,2);
  guivalue(2,s,2,"User","Porter1");
  ( Porter_Order()
  | s[2]>>fc; // Receive flowchart
    link(2,pharmacy_reception,t,1);
    guivalue(2,t,1,"User","Porter1");
    t[2]<<fc; // Deliver flowchart
    end
  )
in ( Porter_Order() | // }}}
// GLOBAL TYPE: $transfer_rejected // {{{
// The sending of a rejected flowchart from the pharmacy
// Users:
//  1: The Controlling Pharmacist
//  2: The Porter
define $transfer_rejected =
  1=>2:2<$flowchart_atcalc@(2of2)>;                  // Porter receives flowchart
  Gend
in // }}}
(nu rejected_send: $transfer_rejected)
(nu rejected_receive: $transfer_rejected)
def Porter_Rejected() = // {{{
  link(2,rejected_send,s,2);
  guivalue(2,s,2,"User","Porter2");
  ( Porter_Rejected()
  | s[2]>>fc; // Receive rejected flowchart
    link(2,rejected_receive,t,1);
  guivalue(2,t,1,"User","Porter2");
    t[2]<<fc; // Deliver rejected flowchart
    end
  )
in ( Porter_Rejected() | // }}}
// GLOBAL TYPE: $transfer_accepted // {{{
// The sending of an accepted flowchart (and the drug) from the pharmacy
// Users:
//  1: The Controlling Pharmacist
//  2: The Porter
define $transfer_accepted =
  1=>2:2<$flowchart_attonurse@(2of2)>;                  // Porter receives flowchart
  1=>2:2<$drug_atcheck2@(2of2)>;                        // Porter receives drug
  Gend
in // }}}
(nu accepted_send: $transfer_accepted)
(nu accepted_receive: $transfer_accepted)
def Porter_Accepted() = // {{{
  link(2,accepted_send,s,2);
  guivalue(2,s,2,"User","Porter3");
  ( Porter_Accepted()
  | s[2]>>theFlowchart;   // Receive flowchart
    s[2]>>theDrug; // Receive drug
    link(2,accepted_receive,t,1);
    guivalue(2,t,1,"User","Porter3");
    t[2]<<theFlowchart;   // Deliver flowchart
    t[2]<<theDrug; // Deliver drug
    end
  )
in ( Porter_Accepted() | // }}}
// }}}
// ONCOLOGY WARD {{{
// GLOBAL TYPE: $oncology_reception {{{
// The reception of a patient
// Users:
//  1: The Patient
//  2: The Doctor
define $oncology_reception =
  1=>2:2<String>;
  {^Register:                // Receive Patient (register information)
    2=>1:1< <$oncology_administer> >;
    {^CalcDosis:
      Gend
    }
  }
in // }}}
(nu oncology_reception: $oncology_reception)
def OncologyDoctor() = // {{{
  link(2,oncology_reception,pa,2);
  guivalue(2,pa,2,"User","Oncology Doctor");
  ( OncologyDoctor()
  | pa[2]>>patient_info;
    guisync(2,pa,2)
    {^Register(patient:String=patient_info,
               height:Int=0,
               weight:Int=0,
               history:String="",
               labresult:String=""):
      link(2,flowchart,fc,2);
      guivalue(2,fc,2,"User","Oncology Doctor");
      fc[1]<<^RegisterPatient;
      fc[1]<<patient;
      fc[1]<<height;
      fc[1]<<weight;
      fc[1]<<history;
      fc[1]<<labresult;
      (nu ref: $oncology_administer)
      fc[1]<<ref;
      pa[1]<<ref;
      guisync(2,pa,2)
      {^CalcDosis(dosis:String=""):
        fc[1]<<^CalcDosis;
        fc[1]<<dosis;
        fc[1]<<^Sign;
        link(2,drug_order,porter,1);
        guivalue(2,porter,1,"User","Oncology Doctor");
        porter[2]<<fc;
        end
      }
    }
  )
in ( OncologyDoctor() | // }}}
(nu oncology_findauth: 1=>2:2< <$oncology_administer> >;Gend)
def OncologyNurse() = // {{{
  link(2,accepted_receive,porter,2);
  guivalue(2,porter,2,"User","Oncology Nurse");
  ( OncologyNurse()
  | def ON_Checks(fc: $flowchart_atcheck3@(2of2), // {{{
                  dr: $drug_atcheck2@(2of2),
                  ref: <$oncology_administer>) =
      link(3,ref,pa,2);     // Find patient from reference
      guivalue(3,pa,2,"User","Oncology Nurse");
      pa[2]>>patient_info;
      guivalue(3,pa,2,"Patient/Info",patient_info);
      fc[1]<<^Check;
      fc[2]>>fc_dosis;
      guivalue(3,pa,2,"Flowchart/Dosis",fc_dosis);      
      fc[2]>>fc_patient;
      guivalue(3,pa,2,"Flowchart/Patient",fc_patient);
      dr[1]<<^Check;
      dr[2]>>drug_dosis;
      guivalue(3,pa,2,"Drug/Dosis",drug_dosis);
      dr[2]>>drug_patient;
      guivalue(3,pa,2,"Drug/Patient",drug_patient);
      guisync(3,pa,2)
      {^Reject(reason:String=""):
        fc[1]<<^Reject;
        dr[1]<<^Reject;
        ON_Checks(fc,dr,ref),
       #Verify(comment:String=""):
        fc[1]<<^Accept;
        pa[4]<<fc;
        dr[1]<<^Verify;
        pa[4]<<dr;
        sync(3,pa)
        {^Reject: // Authorized person rejects
          pa[2]>>fc;
          pa[2]>>dr;
          ON_Checks(fc,dr,ref),
         #Verify: // Authorized person accepts
          pa[2]>>fc;
          pa[2]>>dr;
          guisync(3,pa,2)
          {^Administer(comment:String=""):
            fc[1]<<^Sign;
            dr[1]<<^Administer;
            end
          }
        }
      }
    in // }}}
    porter[2]>>fc;          // Receive Flowchart
    porter[2]>>dr;          // Receive Drug
    fc[2]>>ref;             // Read patient reference from flowchart
    link(2,oncology_findauth,auth,1);
    guivalue(2,auth,1,"User","Oncology Nurse");
    auth[2]<<ref;           // Send patient reference to auth person
    ON_Checks(fc,dr,ref)
  )
in ( OncologyNurse() | // }}}
def OncologyAuthorised() = // {{{
  link(2,oncology_findauth,s,2);
  guivalue(2,s,2,"User","Oncology Auth");
  ( OncologyAuthorised()
  | def OA_Checks(ref: <$oncology_administer>) =
      link(3,ref,pa,3);     // Find patient from reference
      guivalue(3,pa,3,"User","Oncology Auth");
      sync(3,pa)
      {^Reject:
        OA_Checks(ref),
       #Verify:
        pa[3]>>patient_info;
        pa[4]>>fc;
        pa[4]>>dr;
        guivalue(3,pa,3,"Patient/Info",patient_info);
        fc[1]<<^Check;
        fc[2]>>flowchart_dosis;
        guivalue(3,pa,3,"Flowchart/Dosis",flowchart_dosis);
        fc[2]>>flowchart_patient;
        guivalue(3,pa,3,"Flowchart/Patient",flowchart_patient);
        dr[1]<<^Check;
        dr[2]>>drug_dosis;
        guivalue(3,pa,3,"Drug/Dosis",drug_dosis);
        dr[2]>>drug_patient;
        guivalue(3,pa,3,"Drug/Patient",drug_patient);
        guisync(3,pa,3)
        {^Reject(reason:String=""): // Authorized person rejects
          fc[1]<<^Reject;
          dr[1]<<^Reject;
          pa[2]<<fc;
          pa[2]<<dr;
          OA_Checks(ref),
         #Verify(comment:String=""): // Authorized person accepts
          fc[1]<<^Accept;
          dr[1]<<^Verify;
          pa[2]<<fc;
          pa[2]<<dr;
          sync(3,pa)
          {^Administer:
            end
          }
        }
      }
    in
    s[2]>>ref;
    OA_Checks(ref)
  )
in ( OncologyAuthorised() | // }}}
 // }}}
// PHARMACY {{{
// WORKSLIP CHANNEL, TYPE AND IMPLEMENTATION {{{
// GLOBAL TYPE: $workslip // {{{
// This is the Interface of a Working Slip
// Users:
//  1: Workslip object
//  2: User/Owner
define $workslip_atverify =
  2=>1:1
  {^ReadInfo:
    1=>2:2<String>;          // Read Patient Information
    1=>2:2<String>;          // Read Product
    1=>2:2<String>;          // Read Batch Number
    1=>2:2<Int>;             // Read Quantity
    Gend
  }
in define $workslip_atregister =
  2=>1:1
  {^RegisterProducts:
    2=>1:1<String>;          // Set Product
    2=>1:1<String>;          // Set Batch Number
    2=>1:1<Int>;             // Set Quantity
    2=>1:1
    {^Sign:
      $workslip_atverify
    },
   ^Destroy:
    Gend
  }
in define $workslip_atread =
  2=>1:1
  {^ReadDescription:
    1=>2:2<String>;
    $workslip_atregister
  }
in define $workslip =
  2=>1:1
  {^SetDescription:
    2=>1:1<String>;          // Write Description
    $workslip_atread
  }
in // }}}
(nu workslip: $workslip)
// This is the Workslip Service Implementation
def Workslip() = // {{{
  link(2,workslip,s,1);
  guivalue(2,s,1,"User","Workslip");
  ( Workslip()
  | s[1]>>
    {^SetDescription:
      s[1]>>description;
      s[1]>>
      {^ReadDescription:
        s[2]<<description;
        s[1]>>
        {^RegisterProducts:
          s[1]>>product;
          s[1]>>batch;
          s[1]>>quantity;
          s[1]>>
          {^Sign:
            s[1]>>
            {^ReadInfo:
              s[2]<<description;
              s[2]<<product;
              s[2]<<batch;
              s[2]<<quantity;
              end
            }
          },
         ^Destroy:
          end
        }
      }
    }
  )
in (Workslip() | // }}}
// }}}
// GLOBAL TYPE: $pharmacy // {{{
// The cooperation at the pharmacy
// Users:
//  1: The Controlling Pharmacist
//  2: The Pharmacy Assistant
define $pharmacy2 =
  rec $x1.
  {^MakeWorkslip:
    1=>2:2<$workslip_atread@(2of2)>;            // CP sends workslip to PA
    {^MakeDrug:                                 // PA produces drug and sends it to CP
      2=>1:1<$workslip_atverify@(2of2)>;        // PA sends workslip to CP
      2=>1:1<$drug_atcheck1@(2of2)>;            // PA sends drug to CP
      {#AcceptDrug:
        Gend,                                   // The cooperation is complete
       ^RejectDrug:
        $x1                                     // Make new drug
      }
    }
  }
in define $pharmacy_mkdr2 =                     // At MakeDrug (Accepted Dosis)
  {^MakeDrug:                                   // PA produces drug and sends it to CP
    2=>1:1<$workslip_atverify@(2of2)>;          // PA sends workslip to CP
    2=>1:1<$drug_atcheck1@(2of2)>;              // PA sends drug to CP
    {#AcceptDrug:
      Gend,                                     // The cooperation is complete
     ^RejectDrug:
      $pharmacy2                                // Make new drug
    }
  }
in define $pharmacy_acdr2 =                     // At AcceptDrug (Accepted Dosis)
  {#AcceptDrug:
    Gend,                                       // The cooperation is complete
   ^RejectDrug:
    $pharmacy2                                  // Make new drug
  }
in define $pharmacy =
  rec $x0.
  {^RejectDosis:
    Gend,                                       // No internal communication
   #AcceptDosis:
    $pharmacy2,
   #MakeWorkslip:
    1=>2:2<$workslip_atread@(2of2)>;            // CP sends workslip to PA
    {^RejectDosis:
      Gend,
     #AcceptDosis:
      $pharmacy_mkdr2,
     #MakeDrug:
      2=>1:1<$workslip_atverify@(2of2)>;        // PA sends workslip to CP
      2=>1:1<$drug_atcheck1@(2of2)>;            // PA sends drug to CP
      {^RejectDosis:
        Gend,
       #AcceptDosis:
        $pharmacy_acdr2,
       #AcceptDrug:
        {^RejectDosis:
          Gend,
         #AcceptDosis:
          Gend
        },
       #RejectDrug:
        $x0
      }
    }
  }
in define $pharmacy_mkdr =
  {^RejectDosis:
    Gend,
   #AcceptDosis:
    $pharmacy_mkdr2,
   #MakeDrug:
    2=>1:1<$workslip_atverify@(2of2)>;                // PA sends workslip to CP
    2=>1:1<$drug_atcheck1@(2of2)>;                    // PA sends drug to CP
    {^RejectDosis:
      Gend,
     #AcceptDosis:
      $pharmacy_acdr2,
     #AcceptDrug:
      {^RejectDosis:
        Gend,
       #AcceptDosis:
        Gend
      },
     #RejectDrug:
      $pharmacy
    }
  }
in define $pharmacy_acdr =
  {^RejectDosis:
    Gend,
   #AcceptDosis:
    $pharmacy_acdr2,
   #AcceptDrug:
    {^RejectDosis:
      Gend,
     #AcceptDosis:
      Gend
    },
   #RejectDrug:
    $pharmacy
  }
in define $pharmacy_acca =
  {^RejectDosis:
    Gend,
   #AcceptDosis:
    Gend
  }
// *** VERSION USING STATE *** define $pharmacy = // {{{
// *** VERSION USING STATE ***   rec $state<ad:Bool=false,mw:Bool=false,mdr:Bool=false,adr:Bool=false,rd:Bool=false>.
// *** VERSION USING STATE ***   {^RejectDosis[[not (ad or rd)]]:
// *** VERSION USING STATE ***     $state<false,mw,mdr,adr,true>,
// *** VERSION USING STATE ***    #AcceptDosis[[not (ad or rd)]]:
// *** VERSION USING STATE ***     $state<true,mw,mdr,adr,false>,
// *** VERSION USING STATE ***    ^MakeWorkslip[[not (mw or rd)]]:
// *** VERSION USING STATE ***     1=>2:2<$workslip_atread@(2of2)>;                  // CP sends workslip to PA
// *** VERSION USING STATE ***     $state<ad,true,mdr,adr,rd>,
// *** VERSION USING STATE ***    ^MakeDrug[[mw and not (mdr or rd)]]:
// *** VERSION USING STATE ***     2=>1:1<$workslip_atverify@(2of2)>;                // PA sends workslip to CP
// *** VERSION USING STATE ***     2=>1:1<$drug_atcheck1@(2of2)>;                    // PA sends drug to CP
// *** VERSION USING STATE ***     $state<ad,mw,true,adr,rd>,
// *** VERSION USING STATE ***    #AcceptDrug[[mdr and not (adr or rd)]]:
// *** VERSION USING STATE ***     $state<ad,mw,mdr,true,rd>,
// *** VERSION USING STATE ***    ^RejectDrug[[mdr and not (adr or rd)]]:
// *** VERSION USING STATE ***     $state<ad,mw,false,false,rd>,
// *** VERSION USING STATE ***    ^Finish[[(ad and mw and mdr and adr) or rd]]:
// *** VERSION USING STATE ***     Gend
// *** VERSION USING STATE ***   } // }}}
in // }}}
(nu pharmacy: $pharmacy)
def PharmacistAssistant() = // {{{
  link(2,pharmacy,s,2);
  guivalue(2,s,2,"User","Pharmacy Assistant");
  ( PharmacistAssistant()
  | def PA_DestroyWorksheet(reason: String, ws: $workslip_atregister@(2of2)) = // {{{
      (nu accept: {^Accept: Gend})
      link(1,accept,s,1);
      guivalue(1,s,1,"User","Pharmacy Assistant");
      guivalue(1,s,1,"Action:","Destroy Worksheet");
      guivalue(1,s,1,"Reason:",reason);
      guisync(1,s,1)
      {^Accept(comment:String="Worksheet Destroyed"):
        ws[1]<<^Destroy;
        end
      }
    in // }}}
    def PA_Accept(s:$pharmacy2@(2of2)) = // {{{
      guisync(2,s,2)
      {^MakeWorkslip():
        s[2]>>ws;
        ws[1]<<^ReadDescription;
        ws[2]>>description;
        guivalue(2,s,2,"Description:",description);
        guisync(2,s,2)
        {^MakeDrug(patient:String=description,product:String="",batch:String="",quantity:Int=0):
          link(2,drug,d,2);
          guivalue(2,d,2,"User","Pharmacy Assistant");
          d[1]<<^Prepare;
          d[1]<<product;
          d[1]<<patient;
          ws[1]<<^RegisterProducts;
          ws[1]<<product;
          ws[1]<<batch;
          ws[1]<<quantity;
          ws[1]<<^Sign;
          s[1]<<ws;
          s[1]<<d;
          sync(2,s)
          {#AcceptDrug:
            end,
           ^RejectDrug:
            PA_Accept(s)
          }
        }
      }
    in // }}}
    def PA_NoAccept(s:$pharmacy@(2of2)) = // {{{
      guisync(2,s,2)
      {^RejectDosis():
        end,
       #AcceptDosis():
        guivalue(2,s,2,"Dosis","Accepted");
        PA_Accept(s),
       #MakeWorkslip():
        s[2]>>ws;
        ws[1]<<^ReadDescription;
        ws[2]>>description;
        guivalue(2,s,2,"Description:",description);
        guisync(2,s,2)
        {^RejectDosis():
          PA_DestroyWorksheet("Dosis rejected",ws),
         #AcceptDosis():
          guivalue(2,s,2,"Dosis","Accepted");
          guisync(2,s,2)
          {^MakeDrug(patient:String=description,product:String="",batch:String="",quantity:Int=0):
            link(2,drug,d,2);
            guivalue(2,d,2,"User","Pharmacy Assistant");
            d[1]<<^Prepare;
            d[1]<<product;
            d[1]<<patient;
            ws[1]<<^RegisterProducts;
            ws[1]<<product;
            ws[1]<<batch;
            ws[1]<<quantity;
            ws[1]<<^Sign;
            s[1]<<ws;
            s[1]<<d;
            sync(2,s)
            {#AcceptDrug:
              end,
             ^RejectDrug:
              PA_Accept(s)
            }
          },
         #MakeDrug(patient:String=description,product:String="",batch:String="",quantity:Int=0):
          link(2,drug,d,2);
          guivalue(2,d,2,"User","Pharmacy Assistant");
          d[1]<<^Prepare;
          d[1]<<product;
          d[1]<<patient;
          ws[1]<<^RegisterProducts;
          ws[1]<<product;
          ws[1]<<batch;
          ws[1]<<quantity;
          ws[1]<<^Sign;
          s[1]<<ws;
          s[1]<<d;
          sync(2,s)
          {^RejectDosis:
            end,
           #AcceptDosis:
            guivalue(2,s,2,"Dosis","Accepted");
            sync(2,s)
            {#AcceptDrug:
              end,
             ^RejectDrug:
              PA_Accept(s)
            },
           #AcceptDrug:
            sync(2,s)
            {^RejectDosis:
              end,
             #AcceptDosis:
              end
            },
           #RejectDrug:
            PA_NoAccept(s)
          }
        }
      }
    in // }}}
    PA_NoAccept(s)
  )
in ( PharmacistAssistant() | // }}}
def ControllingPharmacist() = // {{{
  link(2,pharmacy_reception,s,2);
  guivalue(2,s,2,"User","Controlling Pharmacist");
  ( ControllingPharmacist()
  | def CP_AtMakeWorkslip2(ph: $pharmacy2@(1of2), // {{{
                           fc: $flowchart_ataccept2no1@(2of2)) =
      // FIXME: Double Def
    def CP_AtAcceptDrug2(ph: $pharmacy_acdr2@(1of2), // {{{
                         fc: $flowchart_ataccept2no1@(2of2),
                         dr: $drug_ataccept1@(2of2)) =
      guisync(2,ph,1)
      {^RejectDrug(comment:String=""):
        dr[1]<<^Destroy;
        fc[1]<<^RejectDrug;
        CP_AtMakeWorkslip2(ph,fc),
       #AcceptDrug(comment:String=""):
        dr[1]<<^Verify;
        fc[1]<<^AcceptDrug;
        fc[1]<<^Sign;
        link(2,accepted_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        porter[2]<<dr;
        end
      }
    in // }}}
      // FIXME: Double Def
    def CP_AtMakeDrug2(ph: $pharmacy_mkdr2@(1of2), // {{{
                       fc: $flowchart_ataccept2no1@(2of2)) =
      guisync(2,ph,1)
      {^MakeDrug():
        ph[1]>>ws;
        ph[1]>>dr;
        // Read info from Worksheet
        ws[1]<<^ReadInfo;
        ws[2]>>ws_patient;
        guivalue(2,ph,1,"Workslip/Patient:",ws_patient);
        ws[2]>>ws_product;
        guivalue(2,ph,1,"Workslip/Product:",ws_product);
        ws[2]>>ws_batch;
        guivalue(2,ph,1,"Workslip/Batch:",ws_batch);
        ws[2]>>ws_quantity;
        guivalue(2,ph,1,"Workslip/Quantity:",ws_quantity);
        // Read info from Drug label
        dr[1]<<^Check;
        dr[2]>>dr_content;
        guivalue(2,ph,1,"Drug/Content:",dr_content);
        dr[2]>>dr_patient;
        guivalue(2,ph,1,"Drug/Patient:",dr_patient);
        CP_AtAcceptDrug2(ph,fc,dr)
      }
    in // }}}
      guisync(2,ph,1)
      {^MakeWorkslip(description:String="Patient:, Dosis:"):
        link(2,workslip,ws,2);
        guivalue(2,ws,2,"User","Controlling Pharmacist");
        ws[1]<<^SetDescription;
        ws[1]<<description;
        ph[2]<<ws;
        CP_AtMakeDrug2(ph,fc)
      }
    in // }}}
    def CP_AtAcceptDrug2(ph: $pharmacy_acdr2@(1of2), // {{{
                         fc: $flowchart_ataccept2no1@(2of2),
                         dr: $drug_ataccept1@(2of2)) =
      guisync(2,ph,1)
      {^RejectDrug(comment:String=""):
        dr[1]<<^Destroy;
        fc[1]<<^RejectDrug;
        CP_AtMakeWorkslip2(ph,fc),
       #AcceptDrug(comment:String=""):
        dr[1]<<^Verify;
        fc[1]<<^AcceptDrug;
        fc[1]<<^Sign;
        link(2,accepted_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        porter[2]<<dr;
        end
      }
    in // }}}
    def CP_AtMakeDrug2(ph: $pharmacy_mkdr2@(1of2), // {{{
                       fc: $flowchart_ataccept2no1@(2of2)) =
      guisync(2,ph,1)
      {^MakeDrug():
        ph[1]>>ws;
        ph[1]>>dr;
        // Read info from Worksheet
        ws[1]<<^ReadInfo;
        ws[2]>>ws_patient;
        guivalue(2,ph,1,"Workslip/Patient:",ws_patient);
        ws[2]>>ws_product;
        guivalue(2,ph,1,"Workslip/Product:",ws_product);
        ws[2]>>ws_batch;
        guivalue(2,ph,1,"Workslip/Batch:",ws_batch);
        ws[2]>>ws_quantity;
        guivalue(2,ph,1,"Workslip/Quantity:",ws_quantity);
        // Read info from Drug label
        dr[1]<<^Check;
        dr[2]>>dr_content;
        guivalue(2,ph,1,"Drug/Content:",dr_content);
        dr[2]>>dr_patient;
        guivalue(2,ph,1,"Drug/Patient:",dr_patient);
        CP_AtAcceptDrug2(ph,fc,dr)
      }
    in // }}}
    def CP_AtMakeWorkslip(ph: $pharmacy@(1of2), // {{{
                          fc: $flowchart_ataccept1@(2of2)) =
    def CP_AtAcceptCalc(ph: {^RejectDosis: Gend, #AcceptDosis: Gend}@(1of2), // {{{
                        fc: $flowchart_ataccept1no2@(2of2),
                        dr: $drug_atcheck2@(2of2)) =
      guisync(2,ph,1)
      {^RejectDosis(reason:String=""):
        // FIXME: reason is not communicated
        dr[1]<<^Destroy;
        fc[1]<<^RejectCalc;
        link(2,rejected_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        end,
       #AcceptDosis(comment:String=""):
        fc[1]<<^AcceptCalc;
        fc[1]<<^Sign;
        link(2,accepted_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        porter[2]<<dr;
        end
      }
    in // }}}
    def CP_AtAcceptDrug(ph: $pharmacy_acdr@(1of2), // {{{
                        fc: $flowchart_ataccept1@(2of2),
                        dr: $drug_ataccept1@(2of2)) =
      guisync(2,ph,1)
      {^RejectDosis(reason:String=""):
        // FIXME: reason is not communicated
        dr[1]<<^Destroy;
        fc[1]<<^RejectCalc;
        link(2,rejected_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        end,
       #AcceptDosis(comment:String=""):
        fc[1]<<^AcceptCalc;
        CP_AtAcceptDrug2(ph,fc,dr),
       #RejectDrug(comment:String=""):
        dr[1]<<^Destroy;
        fc[1]<<^RejectDrug;
        CP_AtMakeWorkslip(ph,fc),
       #AcceptDrug(comment:String=""):
        dr[1]<<^Verify;
        fc[1]<<^AcceptDrug;
        CP_AtAcceptCalc(ph,fc,dr)
      }
    in // }}}
    def CP_AtMakeDrug(ph: $pharmacy_mkdr@(1of2), // {{{
                      fc: $flowchart_ataccept1@(2of2)) =
      guisync(2,ph,1)
      {^RejectDosis(reason:String=""):
        // FIXME: reason is not communicated
        fc[1]<<^RejectCalc;
        link(2,rejected_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        end,
       #AcceptDosis(comment:String=""):
        fc[1]<<^AcceptCalc;
        CP_AtMakeDrug2(ph,fc),
       #MakeDrug():
        ph[1]>>ws;
        ph[1]>>dr;
        // Read info from Worksheet
        ws[1]<<^ReadInfo;
        ws[2]>>ws_patient;
        guivalue(2,ph,1,"Workslip/Patient:",ws_patient);
        ws[2]>>ws_product;
        guivalue(2,ph,1,"Workslip/Product:",ws_product);
        ws[2]>>ws_batch;
        guivalue(2,ph,1,"Workslip/Batch:",ws_batch);
        ws[2]>>ws_quantity;
        guivalue(2,ph,1,"Workslip/Quantity:",ws_quantity);
        // Read info from Drug label
        dr[1]<<^Check;
        dr[2]>>dr_content;
        guivalue(2,ph,1,"Drug/Content:",dr_content);
        dr[2]>>dr_patient;
        guivalue(2,ph,1,"Drug/Patient:",dr_patient);
        CP_AtAcceptDrug(ph,fc,dr)
      }
    in // }}}
      guisync(2,ph,1)
      {^RejectDosis(reason:String=""):
        // FIXME: reason is not communicated
        fc[1]<<^RejectCalc;
        link(2,rejected_send,porter,1);
        guivalue(2,porter,1,"User","Controlling Pharmacist");
        porter[2]<<fc;
        end,
       #AcceptDosis(comment:String=""):
        fc[1]<<^AcceptCalc;
        CP_AtMakeWorkslip2(ph,fc),
       #MakeWorkslip(description:String="Patient: <>, Dosis: <>"):
        link(2,workslip,ws,2);
        guivalue(2,ws,2,"User","Controlling Pharmacist");
        ws[1]<<^SetDescription;
        ws[1]<<description;
        ph[2]<<ws;
        CP_AtMakeDrug(ph,fc)
      }
    in // }}}
    s[2]>>fc;              // Receive Flowchart
    link(2,pharmacy,ph,1); // Create Pharmacy Workflow
    guivalue(2,ph,1,"User","Controlling Pharmacist");
    guivalue(2,ph,1,"User","Controling Pharmacist");
    // Read information from Flowchart
    fc[1]<<^Check;
    fc[2]>>patient;
    guivalue(2,ph,1,"Flowchart/Patient",patient);
    fc[2]>>height;
    guivalue(2,ph,1,"Flowchart/Patient/Height",height);
    fc[2]>>weight;
    guivalue(2,ph,1,"Flowchart/Patient/Weight",weight);
    fc[2]>>history;
    guivalue(2,ph,1,"Flowchart/Patient/History",history);
    fc[2]>>labresult;
    guivalue(2,ph,1,"Flowchart/Patient/Lab. Result",labresult);
    fc[2]>>dosis;
    guivalue(2,ph,1,"Flowchart/Dosis",dosis);
    CP_AtMakeWorkslip(ph,fc)
  )
in (ControllingPharmacist() | // }}}
// }}}
// PATIENT TEMPLATE IMPLEMENTATION {{{
def Patient(myInfo: String) =
  link(2,oncology_reception,doctor,1);
  guivalue(2,doctor,1,"User","Patient");
  doctor[2]<<myInfo;
  sync(2,doctor)
  {^Register:
    doctor[1]>>ref;
    ( sync(2,doctor)
      {^CalcDosis:
        end
      }
    | def P_Wait(ref: <$oncology_administer>) =
        link(3,ref,s,1);
        guivalue(3,s,1,"User","Patient");
        s[2]<<myInfo;
        sync(3,s)
        {^Reject:
          P_Wait(ref),
         #Verify:
          s[3]<<myInfo;
          sync(3,s)
          {^Reject:
            P_Wait(ref),
           #Verify:
            guisync(3,s,1)
            {^Administer(comment:String=""):
              end
            }
          }
        }
      in P_Wait(ref)
    )
  }
in ( // }}}
// Test Patient:
  Patient("Name: A, CPR: A-xxxx")
) ) ) ) ) ) ) ) ) ) ) )
