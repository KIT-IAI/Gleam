/* -----------------------------------------------------------------------------
GLEAM/AE                  Sprach-Header-File (Englisch)   
Package: men                     File: men_gb.h              Version:     V1.7.0
Status : o.k.                   Autor: W.Jakob                 Datum: 30.09.2020
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of this file.
-------------------------------------------------------------------------------- */


/* ----------------------- Texte mehrerer Module: -------------------------- */
#define DYN_GENES_1_TXT    "Gene model based on gene types with unlimited number of integer or"
#define DYN_GENES_2_TXT    "real parameters."


/* ----------------------- Texte des Moduls: f_load ------------------------ */
#define GLEAM_ROOT_OK_TXT  "  The initialization files are also searched in the %s directory: \"%s\""
#define GLEAM_ROOT_ERR_TXT "  The environment variable %s contains no valid directory: \"%s\""
#define NO_GLEAM_ROOT_TXT  "  The environment variable %s does not exist! Therefore no initialization files are searched there."
#define MOD_FILE_SPEC_TXT  "Gene model"
#define BEW_FILE_SPEC_TXT  "Evaluation"
#define TSK_FILE_SPEC_TXT  "Program parameters"
#define EVO_FILE_SPEC_TXT  "Evolution parameters"
#define MEM_FILE_SPEC_TXT  "Chromosome memory"
#define TOP_FILE_SPEC_TXT  "Population topology"
#define SYM_RING_USED      "Symmetrical ring used."
#define TERMINAT_TXT       "Abortion!"


/* ----------------------- Texte des Moduls: men_gsw ------------------------ */
#define EXP_FILE_HDR_TXT   " Experiment File List "
#define EXP_FILE_LINE      "-----------------------"
#define EXP_STD_FILE_TXT   "-------- List of Standard Files: ---------"
#define EXP_APPL_FILE_TXT  "-- List of Application-specific Files: --"
#define ERLEDIGT_TXT       "Done!"
#define FKT_UNVOLLST       "Function may be not completed due to error(s)!"

#define AKTIONS_TXT        ""
#define G_KLASSEN_HDR1     "Number of %sChromosomes per Fitness Class"
#define G_KLASSEN_HDR2     "  Class    Fitness Interval  Number"
#define G_KLASS_BAKS       "From these are %1d basic chromosomes."
#define G_KLASS_FINTXT     "%1d %schromosomes."
#define BAKS_HDR1          "                  %2d Basic Chromosomes:"
#define BAKS_HDR2          "                   (Fitness:Class/Index)"
#define AK_LEN_HDR1        "               Chromosome Lengths:"
#define AK_LEN_HDR2        "---- Fitness -----  #Chr   Chromosome Length"
#define AK_LEN_HDR3        "Class   Interval            min   max   avg"
#define LADE_STAT_HDR      "       Loaded initialization files and status information:"
#define LSTAT_POS_BAD_DATA "Data may be inconsistent because of error in file!"
#define LSTAT_NO_EXP       "No experiment loaded."
#define LSTAT_EXP_TXT      "Experiment      : %s"
#define LSTAT_HMOD_TXT     "Model of actions: %s"
#define LSTAT_GMOD_TXT     "Gene model      : %s"
#define LSTAT_APPL_TXT     "Present applic. : %s"
#define LSTAT_ESIM_MOD     "Model of ExtSim : \"%s\""
#define LSTAT_ESIMPFAD     "ExtSimMod path  : \"%s%s%s\""
#define LSTAT_EXT_SIM      "Simulator       : external simulator: \"%s\""
#define LSTAT_EXT_SIM_SERV "Simulator       : URL of the external services: \"%s\""
#define LSTAT_INT_SIM      "Simulator       : internal simulator"
#define LSTAT_MBF_SIM      "Simulator       : internal benchmark function%s: \"%s\""
#define LSTAT_BEW_TXT      "Evaluation      : %s"
#define LSTAT_MBEW_TXT     "Evaluation(mod) : %s"
#define LSTAT_TSK_TXT      "Program param.  : %s"
#define LSTAT_MTSK_TXT     "Prog.param.(mod): %s"
#define LSTAT_EVO_TXT      "Evo param file  : %s"
#define LSTAT_CHF_TXT      "Chromosome file : %s"
#define LSTAT_NO_CHF       "No chromosome file loaded."
#define LSTAT_TOP_TXT      "Topology (par)  : %s"

#define AK_SCHON_BAK       "Chromosome is already basic chromosome."
#define DEL_BAK_ERG1       "All %1d basic chromosomes changed to chromosomes."
#define DEL_BAK_ERG2       "From these %1d could be deleted."
#define BAKS2AKS_OKTXT     "All %d basic chromosomes changed to chromosomes."
#define BAKS2AKS_F_TXT     "Out of %d basic chromosomes only %d could be changed to chromosomes!"
#define GEN_AK_OK_TXT      "Chromosome stored at address %s."
#define GEN_AK_F1_TXT      "Error saving the chromosome!"
#define GEN_AK_F2_TXT      "Generation of chromosome failed!"
#define AK_TXT             ""
#define ANZG_JOBS_TXT      "     %2u Processed %sOptJobs with:"
#define ANZG_POPUL_TXT     "Populations         : %6d"
#define ANZG_GEN_TXT       "Generations         : %6ld"
#define ANZG_OFFSPRTXT     "Evaluated offsprings: %6ld"
#define ANZG_NOTE_TXT      "Fitness of the best %s: %8.1f  FitnClass: %2u"
#define ANZG_ERGAK_TXT     "Result chromosomes  : %6d"
#define ANZG_ZEIT_TXT      "Total time          : %s"
#define AK_ANZG_TXT        "Chr %2d/%-3d: fitness =%8.1lf"
#define NIX_AK_TXT         "Chromosome %d/%d does not exist!"
#define N_KLASSEN_TXT      "Mutation rates of the %d.fitness class (%ld .. %ld):"
#define MZ_ANZG1_TXT       "%4d ..%5d: %2d"
#define MZ_ANZG2_TXT       "Range for chromosome length "MZ_ANZG1_TXT
#define MZ_ANZG3_TXT       "                            "MZ_ANZG1_TXT
#define SYM_RING_TXT       "symmetrical ring"

/* ----------------------- Texte des Moduls:g_cmen.c  ----------------------- */
#define DO_IT_YESQUERY "  Execute? "JA_NEIN_TXT
#define DO_IT_NO_QUERY "  Execute? "NEIN_JA_TXT
#define FIN_FRAG_TXT   "  Do you really want to quit? "NEIN_JA_TXT

#define LOAD_MEN_ITEM   7
#define SAVE_MEN_ITEM   8
#define SYS_MEN_ITEM    9
#define FIN_MEN_ITEM   12

#define INFO_MEN_TXT   "Info"
#define LOAD_MEN_TXT   "Load"
#define SAVE_MEN_TXT   "Save"
#define SYS_MEN_TXT    "System"
#define FIN_MEN_TXT    "Quit"
#define AK_MEM_MEN_TXT "Chr-Mem"
#define AK_GLO_MEN_TXT "Chr/C glob"
#define AK_INFOMEN_TXT "Chr/C Info"
#define AK_LEN_MEN_TXT "ChrLengths"
#define OPTERG_MEN_TXT "OptResults"
#define LADSTATMEN_TXT "Load State"
#define BAKS_MEN_TXT   "BasicChrs"
#define AK_AUSGMEN_TXT "ChrDisplay"
#define VERS_MEN_TXT   "Version"
#define BAD_DATA_FTXT  "Functions disabled due to inconsistent MOD-, BEW, TSK- or EVO-Data!"
#define LAST_EXP_FAIL  "Error while loading last experiments! Data now consistent?"

#define AK_GLOB_HDR1   "         Chromosomes of a Fitness Class (Overview):"
#define AK_GLOB_HDR2   "       Overview of the Chromosomes of Fitness Class %1d:"
#define AK_GLOB_HDR3   "                   (fitness:index)"
#define GUETE_QUERY    "Class (1..%1d): "
#define AK_INFO_HDR1   "Propertiess of the Chromosomes of a Fitness Class"
#define AK_INFO_HDR2   "Properties of the Chromosomes of Class %1d:"
#ifdef WITH_B_CHRS
  #define AK_INFO_HDR3   "  Addr   Fitness   Len   Segm   BChr  Sim"
#else /* no WITH_B_CHRS */
  #define AK_INFO_HDR3   "  Addr   Fitness   Len   Segm   Sim"
#endif /* no WITH_B_CHRS */
#define AK_AUSG_HDR    "                       Chromosome Display:"
#define OPT_ERG_HDR1   "    Results of the Jobs Processed:"
#define OPT_ERG_HDR2   "            "
#define AK_ERG_QUERY   "Show optimization results? "JA_NEIN_TXT
#define AKERGCLR_QUERY "Reset optimization results? "NEIN_JA_TXT
#define CHIOMODE_QUERY "Standard chromosome file (binary) "JA_NEIN_TXT

#define IO_EXP_MEN     "Experiment"
#define LOAD_HMOD_MEN  "ActionMod "
#define LOAD_GMOD_MEN  "Gene Model"
#define IO_BEW_MEN     "Evaluation"
#define IO_TSK_MEN     "ProgParams"
#define LOAD_EVO_MEN   "EvoParams "
#define IO_MEM_MEN     "Chr-Memory"
#define IO_AKS_MEN     "Chromosoms"
#define LOAD_TOP_MEN   "Topology  "
#define LOAD_EXP_HDR1  "                         Load Experiment:"
#define LOAD_EXP_HDR2  "Experiment \"%s\":"
#define LOAD_EXP_QUERY "Unsaved evo results. Load new experiment anyhow? "NEIN_JA_TXT
#define LOAD_EXPOK_TXT "Experiment"LOAD_OK_TXT
#define LOAD_EXP_F_TXT "Error loading experiment!"
#define LOAD_MIT_ANZGQ "Display loaded chromosomes? "NEIN_JA_TXT
#define LOAD_FILE_OK   "\"%s\""LOAD_OK_TXT
#define LOAD_EXP_MDEL  "Changing the application forces deletion of chromosome memory."
#define LOAD_MOD_HDR   "                         Load Gene Model:"
#define LOAD_BEW_HDR   "                       Load Evaluation Data:"
#define LOAD_TSK_HDR   "                     Load Program Parameters:"
#define LOAD_EVO_HDR   "                    Load Evolution Parameters:"
#define LOAD_MEM_HDR   "                      Load Chromosome Memory:"
#define LOAD_AKS_HDR   "                       Load Chromosome File:"
#define LOAD_APPLF_HDR "                     Load %s:"
#define LOAD_MEM_CLR   "Chromosome memory must be deleted in advance!"
#define LOAD_AFILE_OK  "File"LOAD_OK_TXT

#define SAVE_BAKS_MEN  "Basic Chrs"
#define SAVE_EXP_HDR1  "                         Save Experiment:"
#define SAVE_BEW_HDR   "                         Save Evaluation:"
#define SAVE_TSK_HDR   "                    Save Programm Parameters:"
#define SAVE_MEM_HDR   "                     Save Chromosome Memory:"
#define SAVE_AKS_HDR   "                        Save Chromosomes:"
#define SAVE_BAKS_HDR  "                  Save Basic Chromosomes (BCs):"
#define SAVE_APPLF_HDR "                     Save %s :"
#define SAV_XBEW_TXT   "Evalution possibly changed."
#define SAV_XBEW_QUERY "Does evaluation conform to the one of file \"%s\"? "NEIN_JA_TXT
#define SAV_XBEWQUERY2 "Does evaluation conform to the one of file \"%s\"? "JA_NEIN_TXT
#define SAV_XTSK_TXT   "Program parameters possibly changed."
#define SAV_XTSK_QUERY "Do the program parameters conform to those of file \"%s\"? "NEIN_JA_TXT
#define SAV_XTSKQUERY2 "Do the program parameters conform to those of file \"%s\"? "JA_NEIN_TXT
#define SAV_APPL_QUERY "Overwrite \"%s\"? "JA_NEIN_TXT
#define SAV_NIX_RCODE  "Robot code generation in simulator menu!" 
#define SAV_MEM_QUERY  "Save Chromosome memory? "JA_NEIN_TXT
#define SAVE_INFO1_TXT "Chromosomes to be saved can be marked via the following dialogs."
#define SAVE_INFO2_TXT "Class = 0: End of dialog and saving the marked chromosomes."
#define SAVE_INFO3_TXT "Index = 0: Mark all chromosomes of the typed in fitness class."
#define SAVE_INFO4_TXT "ESC      : Abandon function, reset marks and save nothing."
#define SAVE_AK_QUERY  "Class and index of the chromosome to be saved: "
#define AK_MARKED_TXT  "Chromosome %s has been marked for saving."
#define AKS_MARKED_TXT "All chromosomes of class %d have been marked for saving."
#define SF_BAD_SPEC    "No access to \"%s\"!"
#define SAV_EXP_HDR    "Experiment saved in file \"%s\":"
#define SAV_EXP_ERR    "Errors occured! EXP file not written."
#define SAV_EXP_ERR2   "Error writing EXP file!"
#define SAV_EXP_TXT1   "Experiment"
#define SAV_EXP_TXT2   "from"
#define SAV_NOT_SAVED  "File was not saved!"

#define SYS_MINFO_MEN  "MemInfo"
#define SYS_CHEDI_MEN  "Chr-Editor"
#define SYS_DELMEM_MEN "Del ChrMem"
#define SYS_DEL_G_MEN  "Del Chr/Cl"
#define SYS_DELBAKSMEN "DelAllBCs"
#define SYS_DEL_AK_MEN "Del Chr"
#define SYS_AK2BAK_MEN "Chr->BChr"
#define SYS_AKG2BAKMEN "Chr/Cl->BC"
#define SYS_BAK2AK_MEN "BChr->Chr"
#define SYS_BAKS2AKMEN "BCs-> Chrs"
#define SYS_AK_GEN_MEN "Create Chr"
#define SYS_AKTEST_MEN "Check Chr"
#define SYS_GENREP_MEN "Gen.Repair"
#define SYS_HAM_MEN    "HamDist"
#define SYS_ZEIG_MUTZ  "MutOpRate"
#define IGN_FATAL_MEN  "IgnorFatal"
#define SYS_GOPTST_MEN "GenOp-Test"
#define DELETING_TXT   "Deletion in progress  ..."
#define DEL_AKMEM_HDR  "             Deletion of the Entire Chromosome Memory:"
#define DEL_GCLASS_HDR "          Deletion of all Chromosomes of a Fitness Class:"
#define DEL_BAKS_HDR   "            Deletion of all Basic Chromosomes (BChrs):"
#define DEL_AK_HDR     "                 Deletion of a Chromosome (Chr):"
#define AK2BAK_HDR     "                 Chromosome -> Basic Chromosome:"
#define AKG2BAK_HDR     "          All Chromosome per class -> Basic Chromosomes:"
#define BAK2AK_HDR     "                 Basic Chromosome -> Chromosome:"
#define BAKS2AKS_HDR   "                Basic Chromosomes -> Chromosomes:"
#define AK_GEN_HDR     "                      Chromosome Generation:"
#define AK_TEST_HDR    "              Check for Chromosome Data Consistency:"
#define GEN_REPAIR_HDR "                 Genetic Repair of a Chromosome:"
#define HAM_DIST_HDR   "      Calculation of the Hamming Distance of 2 Chromosomes:"
#define IGN_FATAL_HDR  "                        Reset Error State:"
#define NO_BAK_TXT     "There are no basic chromosomes!"
#define XFER_ERG_TXT   "All %d chromosomes of fitness class %d transfered to basic chromosomes."
#define DEL_BAK_QUERY  "Delete basic chromosomes? "NEIN_JA_TXT
#define DEL_AK_DONETXT "Chromosome deleted!"
#define DEL_AK_NIX_TXT "Chromosome not deleted!"
#define AK_OK_TEXT     "Chromosome data (length and segment structure) ok."
#define GENREP_NIX_TXT "Chromosome cannot be repaired!"
#define HAM_DIST_TXT   "The hamming distance of the two chromosomes is: %6.4f %%"
#define HAM_DIST_TXT2  "The hamming distance of the two chromosomes is: %.*g %%"
#define IGN_FATAL_TXT1 "Error state reset. Caution! Error situation still persists."
#define IGN_FATAL_TXT2 "Function \"MemInfo\" will set error state again!"
#define MZ_ANZG_HDR1   "      Mutation Rate as a Function of Chromosome Length"
#define MZ_ANZG_HDR2   "               Mutation Rate of Operator %d:"
#define MZ_ANZG_QUERY1 "Index of the mutation operator (0..%d): "
#define MZ_ANZG_QUERY2 "Number of displayed chromosome length ranges (4..30) [%d]:"
#define CHR_LOE_ERR    "Error deleting chromosomes in memory and/or population!"
#define OP_TEST_HDR    "                   Test of Genetic Operators:"
#define OP_IDX_QUEWRY  "Index of the genetic operator [-%d..%d]: "
#define FITNESS_TXT    "Fitness values: Parent: %g  Child: %g"
#define CHILD_STORED_1 "Child stored at address"
#define CHILD_STORED_2 "."
#define CHILD_NOT_SIM  "Offspring cannot be simulated!"
#define NO_MUT_DONE    "Offspring was not mutated! Deleted."
#define NO_CHILD_TXT   "No child created!"
#define PARENT_FITN    "Parent fitness   : Parent 1: %g  Parent 2: %g"
#define OFFSPRING_FITN "Offspring fitness: Child 1 : %g  Child 2 : %g"
#define NO_CHILDREN    "No descendants created!"
#define FIRST_TXT      "1st "
#define SECOND_TXT     "2nd "
