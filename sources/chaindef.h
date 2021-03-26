/* -----------------------------------------------------------------------------
GLEAM/AE                       Global Header-File
Package: lgsw                    File: chaindef.h                Version: V2.0.0
--------------------------------------------------------------------------------
This is free software, which is governed by the Lesser GNU Public Licence (LGPL), 
version 3, see the comment section at the end of the main program files or the 
files "schalter.h".
--------------------------------------------------------------------------------
 
Definition of the data types of all kinds of chains. A chain is a single pointer 
list with a list header. In this include file the data types for all chain types 
are defined. In the current GLEAM context only linear action chains (ACs, in German 
AKs) and administration chains (VKs) are used. Action chains are chromosomes inter-
preted with a time reference. Administration chains are used to manage the chromo-
some memory.

Action chains or chromosomes have a header parameter element, which can be accessed 
via the "header_par_ptr". For the other chain types this pointer is NULL. The seg-
mentation of chromosomes is managed by segment descriptors, which also represent a 
linear list, whose beginning is stored in the chain header. A segment descriptor 
contains a list of descriptor elements consisting of two entries: a pointer to the 
beginning of the segment and its length.

--------------------------------------------------------------------------------
List of changes:
 1. I_PAR_GES_ANZ_MAX und R_PAR_GES_ANZ_MAX entfernt.                WJ 13.08.00
 2. Neuer Kettenstatus RANDOMLY_GENERATED eingefuehrt.               WJ 09.02.06
 3. Anpassung an 64 bit: Kettenelemente haben 128 Bytes bei groes-
    seren Datentypen fuer I_PARAMS_TYPE, R_PARAMS_TYPE und 
    SIM_ERG_TYPE (SHORT -> INT und SINGLE -> DOUBLE).                WJ 03.01.08
 4. Streichung von "next_mblock" der alten DOS-Speicherverwaltung.   WJ 21.04.08
 5. MPI-Version: Neuer Kopfeintrag "parIndIdx".                      WJ 18.10.12
 6. Renovierung: Streichung von SEGM_ANZ_MAX.                        WJ 18.04.16
 7. Integrated protection against multiple calls.                    WJ 13.07.16
 8. New switch WITH_DYN_GENES for gene types with unlimited number 
    of gene parameters. Introduction of MAX_GENE_CODE.               WJ 01.07.19
--------------------------------------------------------------------------------
Status: O.K.                    Author: W.Jakob                 Date: 01.07.2019
******************************************************************************** */

#ifndef CHAINDEF_H
#define CHAINDEF_H

/* ---------------------------- activity-Codes: ----------------------------- */
#define ACTIVITY_ACT 	                 0  /* sub-chain reference            */
#define MAX_GENE_CODE                30000  /* max. gene code or activity     */
#define VK_HEADER      MAX_GENE_CODE + 100  /* header of administrative chain */
#define AK_HEADER      MAX_GENE_CODE + 200  /* hdr of action chain/chromosome */


/* ---------------------------- "ref_typ"-Codes: ---------------------------- */
#define LOC_SUB_AK         -1  /* "activity_act"-Item has local sub chain     */
#define SUB_AK_REF         -2  /* "activity_act"-Item has sub chain reference */
#define VK_ITEM_REF       -10  /* "activity_act"-Item is in VK witk AK-refer. */


/* ------------------ parameterization of a gene / action: ------------------ */
#define I_PAR_ANZ_MAX 	   12 /* maximum number of integer parameters per gene*/
#define R_PAR_ANZ_MAX 	    8 /* maximum number of real parameters per gene   */


/* ----------------------------- other limits: ------------------------------ */
#define SEGM_PTR_ANZ       12 /* number of segment pointers per element       */
#define AK_ERG_WERTE_MAX   16 /* max. number of "sim_erg_werte" stored in  AK */


/* ---------------- special fitness and fitness class values: --------------- */
#define KEINE_NOTE     999999.0 /* invalid fitness value                      */
#define RNOTEN_MAX     100000.0 /* greates fitness value (DOUBLE)             */
#define INOTEN_MAX     100000L  /* greates fitness value (LONG)               */
#define KEINE_GUETE 	  999   /* invalid fitness class value                */
#define MAX_GUETE          16   /* best fitness class. Interval: 1..MAX_GUETE */
#define BASIS_AK_GUETE      0   /* fitness class for basic chromosomes        */


/* ------------------------ codes of the chromosome status: ----------------------- */
#define BASIS_AK                  1
#define TO_BE_SAVED               2
#define SIMULATED                 4
#define RANDOMLY_GENERATED        8
#define UNUSED_5                 16
#define UNUSED_6                 32
#define UNUSED_7                 64
#define UNUSED_8                128
#define UNUSED_9                256
#define UNUSED_10               512
#define UNUSED_11              1024
#define UNUSED_12              2048
#define UNUSED_13              4096
#define UNUSED_14              8192
#define UNBEWERTETE_BASIS_AK  16384
#define UNMUTIERTE_URKETTE    32768


typedef INT     I_PARAMS_TYPE [I_PAR_ANZ_MAX];

typedef DOUBLE  R_PARAMS_TYPE [R_PAR_ANZ_MAX];

typedef DOUBLE  SIM_ERG_TYPE  [AK_ERG_WERTE_MAX];

typedef union   CHAIN_ELEM_TYPE *CHAIN_ELEM_PTR_TYPE;

typedef struct  REF_DESCR_TYPE *REF_DESCR_PTR_TYPE;


typedef struct REF_DESCR_TYPE 
  { REF_DESCR_PTR_TYPE   next_ref_descr_ptr;
    CHAIN_ELEM_PTR_TYPE  segm_ptrs   [SEGM_PTR_ANZ];
    SHORT                segm_length [SEGM_PTR_ANZ];
  } REF_DESCR_TYPE;


typedef union CHAIN_ELEM_TYPE 
   { struct 
      { SHORT                activity;               /* gene ID               */
        CHAIN_ELEM_PTR_TYPE  next_action;            /* next element          */
        union 
           { struct 
              { /* ---------------------- Header-Item ----------------------- */
                CHAIN_ELEM_PTR_TYPE  last_action;    /* ptr to last element   */
                CHAIN_ELEM_PTR_TYPE  header_par_ptr; /* ptr to hdr param elem */
                REF_DESCR_PTR_TYPE   segm_descr_ptr; /* ptr to segment descr. */
                DOUBLE               fitness_note;   /* fitness               */
                SHORT                ref_counter;    /* # of references to it */
                SHORT                chain_length;   /* # of chain elements   */
                SHORT                guete,          /* fitness class         */
                                     lfd_nr;         /* index part of chr.adr.*/
                SHORT                segment_anz;    /* number of segments    */
                USHORT               chain_status;   /* status of the chain   */
	        SHORT                ak_index,       /* used for chain-I/O    */
                                     parIndIdx;      /* used for chain-I/O    */
              } u_hdr;

            struct 
              { /* ------------------- Activity-Act-Item -------------------- */
                CHAIN_ELEM_PTR_TYPE  sub_chain;       /* pointer to sub chain */
                SHORT                ref_typ;         /* reference type       */
                SHORT                sub_ak_guete,
                                     sub_ak_lfd_nr,
                                     sub_ak_index;
              } u_sub;

            struct 
              { /* ----------------- Action- or Gene-Item ------------------- */
#ifdef WITH_DYN_GENES
		 INT           *i_params;
		 DOUBLE        *r_params;
#else /* static genes */
		 I_PARAMS_TYPE  i_params;  /* static field of int gene params */
                 R_PARAMS_TYPE  r_params;  /* static field of real gene params*/
#endif /* static genes */
              } u_act;
          } u_xel;
      } u_el;
  
    /* --------------------------- Header-Parameter ------------------------- */
    SIM_ERG_TYPE         sim_erg_werte;    /* results of the last simulation  */  
  } CHAIN_ELEM_TYPE;
#endif /* CHAINDEF_H */
