/****************************************************************************************/
/*  CFILE.H                                                                             */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description:                                                                        */
/*                                                                                      */
/*  The contents of this file are subject to the Jet3D Public License                   */
/*  Version 1.02 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.jet3d.com                                                                */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Jet3D, released December 12, 1999.                             */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/

/* Configuration file data chunks */

#define CMAGIC 0xC23D	/* Configuration magic */

#define C_MDRAWER 0xC010
#define C_TDRAWER 0xC020
#define C_SHPDRAWER 0xC030
#define C_MODDRAWER 0xC040
#define C_RIPDRAWER 0xC050
#define C_TXDRAWER 0xC060
#define C_PDRAWER 0xC062
#define C_MTLDRAWER 0xC064
#define C_FLIDRAWER 0xC066
#define C_CUBDRAWER 0xC067
#define C_MFILE 0xC070
#define C_SHPFILE 0xC080
#define C_MODFILE 0xC090
#define C_RIPFILE 0xC0A0
#define C_TXFILE 0xC0B0
#define C_PFILE 0xC0B2
#define C_MTLFILE 0xC0B4
#define C_FLIFILE 0xC0B6
#define C_PALFILE 0xC0B8
#define C_TX_STRING 0xC0C0
#define C_CONSTS 0xC0D0
#define C_SNAPS 0xC0E0
#define C_GRIDS 0xC0F0
#define C_ASNAPS 0xC100
#define C_GRID_RANGE 0xC110
#define C_RENDTYPE 0xC120
#define C_PROGMODE 0xC130
#define C_PREVMODE 0xC140
#define C_MODWMODE 0xC150
#define C_MODMODEL 0xC160
#define C_ALL_LINES 0xC170
#define C_BACK_TYPE 0xC180
#define C_MD_CS 0xC190
#define C_MD_CE 0xC1A0
#define C_MD_SML 0xC1B0
#define C_MD_SMW 0xC1C0
#define C_LOFT_WITH_TEXTURE 0xC1C3
#define C_LOFT_L_REPEAT 0xC1C4
#define C_LOFT_W_REPEAT 0xC1C5
#define C_LOFT_UV_NORMALIZE 0xC1C6
#define C_WELD_LOFT 0xC1C7
#define C_MD_PDET 0xC1D0
#define C_MD_SDET 0xC1E0
#define C_RGB_RMODE 0xC1F0
#define C_RGB_HIDE 0xC200
#define C_RGB_MAPSW 0xC202
#define C_RGB_TWOSIDE 0xC204
#define C_RGB_SHADOW 0xC208
#define C_RGB_AA 0xC210
#define C_RGB_OVW 0xC220
#define C_RGB_OVH 0xC230
#define C_RGB_PICTYPE 0xC240
#define C_RGB_OUTPUT 0xC250
#define C_RGB_TODISK 0xC253
#define C_RGB_COMPRESS 0xC254
#define C_JPEG_COMPRESSION 0xC255
#define C_RGB_DISPDEV 0xC256
#define C_RGB_HARDDEV 0xC259
#define C_RGB_PATH 0xC25A
#define C_BITMAP_DRAWER 0xC25B
#define C_RGB_FILE 0xC260
#define C_RGB_OVASPECT 0xC270

#define C_RGB_ANIMTYPE 0xC271
#define C_RENDER_ALL 0xC272
#define C_REND_FROM 0xC273
#define C_REND_TO 0xC274
#define C_REND_NTH 0xC275
#define C_PAL_TYPE 0xC276
#define C_RND_TURBO 0xC277
#define C_RND_MIP	0xC278
#define C_BGND_METHOD 0xC279
#define C_AUTO_REFLECT 0xC27A
#define C_VP_FROM 0xC27B
#define C_VP_TO 0xC27C
#define C_VP_NTH 0xC27D

#define C_SRDIAM 0xC280
#define C_SRDEG 0xC290
#define C_SRSEG 0xC2A0
#define C_SRDIR 0xC2B0
#define C_HETOP 0xC2C0
#define C_HEBOT 0xC2D0
#define C_HEHT 0xC2E0
#define C_HETURNS 0xC2F0
#define C_HEDEG 0xC300
#define C_HESEG 0xC310
#define C_HEDIR 0xC320
#define C_QUIKSTUFF 0xC330
#define C_SEE_LIGHTS 0xC340
#define C_SEE_CAMERAS 0xC350
#define C_SEE_3D 0xC360
#define C_MESHSEL 0xC370
#define C_MESHUNSEL 0xC380
#define C_POLYSEL 0xC390
#define C_POLYUNSEL 0xC3A0
#define C_SHPLOCAL 0xC3A2
#define C_MSHLOCAL 0xC3A4
#define C_NUM_FORMAT 0xC3B0
#define C_ARCH_DENOM 0xC3C0
#define C_IN_DEVICE 0xC3D0
#define C_MSCALE 0xC3E0
#define C_COMM_PORT 0xC3F0
#define C_TAB_BASES 0xC400
#define C_TAB_DIVS 0xC410
#define C_MASTER_SCALES 0xC420
#define C_SHOW_1STVERT 0xC430
#define C_SHAPER_OK 0xC440
#define C_LOFTER_OK 0xC450
#define C_EDITOR_OK 0xC460
#define C_KEYFRAMER_OK 0xC470
#define C_PICKSIZE 0xC480
#define C_MAPTYPE 0xC490
#define C_MAP_DISPLAY 0xC4A0
#define C_TILE_XY 0xC4B0
#define C_MAP_XYZ 0xC4C0
#define C_MAP_SCALE 0xC4D0
#define C_MAP_MATRIX_OLD 0xC4E0
#define C_MAP_MATRIX 0xC4E1
#define C_MAP_WID_HT 0xC4F0
#define C_OBNAME 0xC500
#define C_CAMNAME 0xC510
#define C_LTNAME 0xC520
#define C_CUR_MNAME 0xC525
#define C_CURMTL_FROM_MESH 0xC526
#define C_GET_SHAPE_MAKE_FACES 0xC527
#define C_DETAIL 0xC530
#define C_VERTMARK 0xC540
#define C_MSHAX 0xC550
#define C_MSHCP 0xC560
#define C_USERAX 0xC570
#define C_SHOOK 0xC580
#define C_RAX 0xC590
#define C_STAPE 0xC5A0
#define C_LTAPE 0xC5B0
#define C_ETAPE 0xC5C0
#define C_KTAPE 0xC5C8
#define C_SPHSEGS 0xC5D0
#define C_GEOSMOOTH 0xC5E0
#define C_HEMISEGS 0xC5F0
#define C_PRISMSEGS 0xC600
#define C_PRISMSIDES 0xC610
#define C_TUBESEGS 0xC620
#define C_TUBESIDES 0xC630
#define C_TORSEGS 0xC640
#define C_TORSIDES 0xC650
#define C_CONESIDES 0xC660
#define C_CONESEGS 0xC661
#define C_NGPARMS 0xC670
#define C_PTHLEVEL 0xC680
#define C_MSCSYM 0xC690
#define C_MFTSYM 0xC6A0
#define C_MTTSYM 0xC6B0
#define C_SMOOTHING 0xC6C0
#define C_MODICOUNT 0xC6D0
#define C_FONTSEL 0xC6E0
#define C_TESS_TYPE 0xC6f0
#define C_TESS_TENSION 0xC6f1

#define C_SEG_START 0xC700
#define C_SEG_END 0xC705
#define C_CURTIME 0xC710
#define C_ANIMLENGTH 0xC715
#define C_PV_FROM 0xC720
#define C_PV_TO 0xC725
#define C_PV_DOFNUM 0xC730
#define C_PV_RNG 0xC735
#define C_PV_NTH 0xC740
#define C_PV_TYPE 0xC745
#define C_PV_METHOD 0xC750
#define C_PV_FPS 0xC755
#define C_VTR_FRAMES 0xC765
#define C_VTR_HDTL 0xC770
#define C_VTR_HD 0xC771
#define C_VTR_TL 0xC772
#define C_VTR_IN 0xC775
#define C_VTR_PK 0xC780
#define C_VTR_SH 0xC785

/* Material chunks */

#define C_WORK_MTLS 0xC790	  /* Old-style -- now ignored */
#define C_WORK_MTLS_2 0xC792 /* Old-style -- now ignored */
#define C_WORK_MTLS_3 0xC793 /* Old-style -- now ignored */
#define C_WORK_MTLS_4 0xC794 /* Old-style -- now ignored */
#define C_WORK_MTLS_5 0xCB00 /* Old-style -- now ignored */
#define C_WORK_MTLS_6 0xCB01 /* Old-style -- now ignored */
#define C_WORK_MTLS_7 0xCB02 /* Old-style -- now ignored */
#define C_WORK_MTLS_8 0xCB03 /* Old-style -- now ignored */
#define C_WORKMTL 0xCB04
#define C_SXP_TEXT_DATA 0xCB10
#define C_SXP_TEXT2_DATA 0xCB20
#define C_SXP_OPAC_DATA 0xCB11
#define C_SXP_BUMP_DATA 0xCB12
#define C_SXP_SPEC_DATA 0xCB24
#define C_SXP_SHIN_DATA 0xCB13
#define C_SXP_SELFI_DATA 0xCB28
#define C_SXP_TEXT_MASKDATA 0xCB30
#define C_SXP_TEXT2_MASKDATA 0xCB32
#define C_SXP_OPAC_MASKDATA 0xCB34
#define C_SXP_BUMP_MASKDATA 0xCB36
#define C_SXP_SPEC_MASKDATA 0xCB38
#define C_SXP_SHIN_MASKDATA 0xCB3A
#define C_SXP_SELFI_MASKDATA 0xC3C
#define C_SXP_REFL_MASKDATA 0xCB3E

#define C_BGTYPE 0xC7A1
#define C_MEDTILE 0xC7B0

/* Contrast */

#define C_LO_CONTRAST 0xC7D0
#define C_HI_CONTRAST 0xC7D1

/* 3d frozen display */

#define C_FROZ_DISPLAY 0xC7E0

/* Booleans */
#define C_BOOLWELD 0xc7f0
#define C_BOOLTYPE 0xc7f1

#define C_ANG_THRESH 0xC900
#define C_SS_THRESH 0xC901
#define C_TEXTURE_BLUR_DEFAULT 0xC903

#define C_MAPDRAWER 0xCA00
#define C_MAPDRAWER1 0xCA01
#define C_MAPDRAWER2 0xCA02
#define C_MAPDRAWER3 0xCA03
#define C_MAPDRAWER4 0xCA04
#define C_MAPDRAWER5 0xCA05
#define C_MAPDRAWER6 0xCA06
#define C_MAPDRAWER7 0xCA07
#define C_MAPDRAWER8 0xCA08
#define C_MAPDRAWER9 0xCA09
#define C_MAPDRAWER_ENTRY 0xCA10

/* system options */
#define C_BACKUP_FILE 0xCA20
#define C_DITHER_256 0xCA21
#define C_SAVE_LAST 0xCA22
#define C_USE_ALPHA 0xCA23
#define C_TGA_DEPTH 0xCA24
#define C_REND_FIELDS 0xCA25
#define C_REFLIP 0xCA26
#define C_SEL_ITEMTOG 0xCA27
#define C_SEL_RESET 0xCA28
#define C_STICKY_KEYINF 0xCA29
#define C_WELD_THRESHOLD 0xCA2A
#define C_ZCLIP_POINT 0xCA2B
#define C_ALPHA_SPLIT 0xCA2C
#define C_KF_SHOW_BACKFACE 0xCA30
#define C_OPTIMIZE_LOFT 0xCA40
#define C_TENS_DEFAULT 0xCA42
#define C_CONT_DEFAULT 0xCA44
#define C_BIAS_DEFAULT 0xCA46

#define C_DXFNAME_SRC  0xCA50
#define C_AUTO_WELD  0xCA60
#define C_AUTO_UNIFY  0xCA70
#define C_AUTO_SMOOTH  0xCA80
#define C_DXF_SMOOTH_ANG  0xCA90
#define C_SMOOTH_ANG  0xCAA0

/* Special network-use chunks */

#define C_NET_USE_VPOST 0xCC00
#define C_NET_USE_GAMMA 0xCC10
#define C_NET_FIELD_ORDER 0xCC20

#define C_BLUR_FRAMES 0xCD00
#define C_BLUR_SAMPLES 0xCD10
#define C_BLUR_DUR 0xCD20
#define C_HOT_METHOD 0xCD30
#define C_HOT_CHECK 0xCD40
#define C_PIXEL_SIZE 0xCD50
#define C_DISP_GAMMA 0xCD60
#define C_FBUF_GAMMA 0xCD70
#define C_FILE_OUT_GAMMA 0xCD80
#define C_FILE_IN_GAMMA 0xCD82
#define C_GAMMA_CORRECT 0xCD84
#define C_APPLY_DISP_GAMMA 0xCD90  /* OBSOLETE */
#define C_APPLY_FBUF_GAMMA 0xCDA0  /* OBSOLETE */
#define C_APPLY_FILE_GAMMA 0xCDB0  /* OBSOLETE */
#define C_FORCE_WIRE 0xCDC0
#define C_RAY_SHADOWS 0xCDD0
#define C_MASTER_AMBIENT 0xCDE0
#define C_SUPER_SAMPLE 0xCDF0
#define C_OBJECT_MBLUR 0xCE00
#define C_MBLUR_DITHER 0xCE10
#define C_DITHER_24 0xCE20
#define C_SUPER_BLACK 0xCE30
#define C_SAFE_FRAME 0xCE40
#define C_VIEW_PRES_RATIO 0xCE50
#define C_BGND_PRES_RATIO 0xCE60
#define C_NTH_SERIAL_NUM 0xCE70
