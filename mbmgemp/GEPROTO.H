/***************************************************************************
 *                                                                         *
 *   GEPROTO.H                                                             *
 *                                                                         *
 *   Copyright (C) 1988, 89, 90, 91, 92 Michael B. Murdock                 *
 *                                                                         *
 *   This is the source for the Galactic Empire game module                *
 *                                                                         *
 *                                         M. Murdock     03/17/92         *
 *                                                                         *
 *   ge-next                                                               *
 *                                                                         *
 *   Copyright (C) 2024-2025 Anthony Schmidt, anthony@manicpop.org         *
 *                                                                         *
 *   https://manicpop.org/ge-next/  https://github.com/manicpop/ge-next    *
 *                                                                         *
 ***************************************************************************/

 /**************************************************************************
  * This program is free software; you can redistribute it and/or modify   *
  * under the terms of the GNU General Public License as published by the  *
  * Free Software Foundation; either version 2 of the License, or (at your *
  * option) any later version.                                             *
  *                                                                        *
  * This program is distributed in the hope that it will be useful,        *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
  * General Public License for more details.                               *
  *                                                                        *
  * You should have received a copy of the GNU General Public License      *
  * along with this program; if not, write to the Free Software Foundation,*
  * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA            *
  *************************************************************************/

#ifdef __STDC__
# define P(s) s
#else
# define P(s) ()
#endif


/* gemain.c */
/*     335 */ void EXPORT init__galemp P((void ));
/*     349 */ int FUNC iniwar P((void ));
/*     356 */ void FUNC dummy P((void ));
/*     360 */ void FUNC iniwara P((void ));
/*     860 */ int FUNC gelogon P((void ));
/*     888 */ void FUNC pgedelete P((char *uid ));
/*     895 */ int gedelete P((char *uid ));
/*     903 */ void FUNC gedeletea P((char *uid ));
/*     930 */ void FUNC pgemidnight P((void ));
/*     936 */ int FUNC gemidnight P((void ));
/*     943 */ void FUNC gemidnighta P((void ));
/*    1182 */ void FUNC calc_networth P((void ));
/*    1190 */ unsigned FUNC long value_pl P((void ));
/*    1209 */ int FUNC pwarlof P((void ));
/*    1225 */ void FUNC pwarhup P((void ));
/*    1232 */ int FUNC warhup P((void ));
/*    1239 */ void FUNC warhupa P((void ));
/*    1285 */ void FUNC pclswar P((void ));
/*    1291 */ int FUNC clswar P((void ));
/*    1298 */ void FUNC clswara P((void ));
/*    1319 */ int FUNC galemp P((void ));
/*    1344 */ void FUNC outwar P((int filter , unsigned exclude , unsigned freq ));
/*    1376 */ int FUNC gepdb P((int func , char *usrname , int shipnum , WARSHP *geptr ));
/*    1458 */ int FUNC geudb P((int func , char *usrname , WARUSR *geptr ));
/*    1533 */ int FUNC gesdb P((int func , PKEY *sect , GALSECT *geptr ));
/*    1618 */ int FUNC getplanetdat P((int usrn ));
/*    1675 */ void FUNC load_team_tab P((void ));
/*    1743 */ void FUNC update_team_tab P((void ));
/*    1776 */ void FUNC pplarti P((void ));
/*    1782 */ int FUNC plarti P((void ));
/*    1790 */ void FUNC plartia P((void ));
/*    2055 */ void FUNC pwarrti P((void ));
/*    2061 */ int FUNC warrti P((void ));
/*    2068 */ void FUNC warrtia P((void ));
/*    2120 */ void FUNC pautorti P((void ));
/*    2126 */ int FUNC autorti P((void ));
/*    2133 */ void FUNC autortia P((void ));
/*    2265 */ void FUNC pwarrti2 P((void ));
/*    2272 */ int FUNC warrti2 P((void ));
/*    2279 */ void FUNC warrti2a P((void ));
/*    2320 */ void FUNC pwarrti3 P((void ));
/*    2327 */ int FUNC warrti3 P((void ));
/*    2334 */ void FUNC warrti3a P((void ));
/*    2364 */ void FUNC outprfge P((int class , int shpno ));
/*    2400 */ void FUNC outsect P((int filter , COORD *coordptr , unsigned exclude , unsigned freq ));
/*    2438 */ void FUNC outrange P((int filter , COORD *coordptr ));
/*    2467 */ int FUNC ingegame P((int shpno ));
/*    2491 */ void FUNC geshocst P((int opt , char *str ));
/*    2540 */ int FUNC mnu_main P((void ));
/*    2551 */ int FUNC mnu_main_ans P((void ));
/*    2664 */ int FUNC mnu_fightsub P((void ));
/*    2707 */ int FUNC mnu_admenu1 P((void ));
/*    2763 */ int mnu_admenu1a P((void ));
/*    2798 */ int FUNC mnu_admenu2 P((void ));
/*    2897 */ int FUNC mnu_admenu2b P((void ));
/*    2938 */ int FUNC mnu_admenu2e P((void ));
/*    2965 */ int FUNC mnu_admenu2f1 P((void ));
/*    2995 */ int FUNC mnu_admenu2f2 P((void ));
/*    3026 */ int FUNC mnu_admenu2f3 P((void ));
/*    3054 */ int FUNC mnu_admenu2f4 P((void ));
/*    3086 */ int FUNC mnu_admenu2h P((void ));
/*    3114 */ int FUNC mnu_admenu2i P((void ));
/*    3169 */ int FUNC mnu_admenu2j P((void ));
/*    3201 */ int FUNC mnu_choosesh P((void ));
/*    3210 */ int mnu_menug P((void ));
/*    3273 */ int FUNC mnu_menug1 P((void ));
/*    3320 */ int FUNC mnu_menug2 P((void ));
/*    3365 */ void FUNC disp_main_menu P((void ));
/*    3383 */ void FUNC disp_menu_d P((void ));
/*    3398 */ void FUNC update_items P((void ));
/*    3438 */ void FUNC optdisp P((void ));
/*    3488 */ void FUNC stshdlr P((void ));

/* gefuncs.c */
/*      71 */ void FUNC lookupshp P((void ));
/*     141 */ void FUNC tossingegame P((void ));
/*     169 */ int FUNC initshp P((char *userid , int type ));
/*     269 */ int FUNC initusr P((char *userid ));
/*     301 */ int FUNC findships P((int direction , int quiet ));
/*     349 */ void FUNC selectship P((void ));
/*     387 */ void FUNC repairship P((WARSHP *ptr , int usrn ));
/*     434 */ void FUNC rotateship P((WARSHP *ptr , int usrn ));
/*     473 */ void FUNC accel P((WARSHP *ptr , int usrn ));
/*     565 */ void FUNC hyperspace P((WARSHP *ptr , int usrn , int flag ));
/*     633 */ void FUNC moveship P((WARSHP *ptr , int usrn ));
/*     844 */ void FUNC telezip P((WARSHP *ptr , int usrn ));
/*     875 */ void FUNC gravity P((WARSHP *ptr , int usrn ));
/*     949 */ void FUNC checkdist P((WARSHP *ptr , int usrn ));
/*     973 */ void FUNC refresh P((WARSHP *ptr , int usrn ));
/*    1012 */ void FUNC checkdam P((WARSHP *ptr , int usrn ));
/*    1126 */ void FUNC killem P((WARSHP *ptr , int usrn ));
/*    1414 */ void FUNC recharge P((WARSHP *ptr , int usrn ));
/*    1431 */ int FUNC fluxstat P((WARSHP *ptr , int usrn , int energy ));
/*    1466 */ void FUNC shieldstat P((WARSHP *ptr , int usrn ));
/*    1498 */ void FUNC cloakstat P((WARSHP *ptr , int usrn ));
/*    1535 */ void FUNC checkmines P((void ));
/*    1660 */ void FUNC checktm P((WARSHP *ptr , int usrn ));
/*    1903 */ void FUNC acctm P((WARSHP *ptr , int usrn , int mt , unsigned int channel ));
/*    1926 */ int FUNC chkitm P((int usrn ));
/*    1926 */ void FUNC clearitm P((int usrn ));
/*    1926 */ void FUNC cleartm P((int channel ));
/*    1961 */ void FUNC fireion P((WARSHP *ptr , int usrn ));
/*    1996 */ void FUNC destruct P((WARSHP *ptr , int usrn ));
/*    2082 */ int FUNC valpcnt P((char *ptr , unsigned minnum , unsigned maxnum ));
/*    2113 */ int FUNC valdegree P((char *ptr ));
/*    2136 */ void FUNC randamage P((WARSHP *ptr , int usrn ));
/*    2240 */ unsigned FUNC pdamage P((WARSHP *wptr , double dist , int foc ));
/*    2317 */ void FUNC setsect P((WARSHP *ptr ));
/*    2333 */ void FUNC movecoord P((COORD *pointb , COORD *pointa ));
/*    2346 */ int FUNC samesect P((COORD *pointb , COORD *pointa ));
/*    2363 */ int FUNC genearas P((char *str1 , char *str2 ));
/*    2378 */ int FUNC mailscan P((char *userid , int class ));
/*    2407 */ int mailread P((char *userid , int class ));
/*    2439 */ void FUNC mailit P((int flag ));
/*    2533 */ int FUNC sendit P((void ));
/*    2564 */ void FUNC prf2tx P((void ));
/*    2585 */ int FUNC sendgemsg P((struct message *msgptr , char *usrid ));
/*    2605 */ void FUNC shieldup P((WARSHP *wptr , int usrn ));
/*    2615 */ void FUNC shielddn P((WARSHP *wptr , int usrn ));
/*    2626 */ int FUNC shieldhit P((WARSHP *wptr , int usrn , int dam ));
/*    2668 */ void FUNC shieldrep P((WARSHP *wptr , int usrn ));
/*    2686 */ void FUNC shieldchg P((WARSHP *wptr , int usrn ));
/*    2725 */ void FUNC charge P((WARSHP *wptr , int *max , int *pct ));
/*    2736 */ int FUNC chkweight P((WARSHP *wptr , int itm , long amt ));
/*    2757 */ unsigned long FUNC calcweight P((WARSHP *wptr ));
/*    2776 */ char FUNC shpltr P((int usrn , int ship ));
/*    2794 */ char *FUNC username P((WARSHP *ptr ));
/*    2806 */ void FUNC logthis P((char *str ));
/*    2836 */ WARUSR *FUNC warusroff P((int usrn ));
/*    2847 */ WARSHP *FUNC warshpoff P((int usrn ));
/*    2859 */ double FUNC ton_fact P((WARSHP *ptr , double damfact ));
/*    2872 */ char *FUNC showarp P((double speed ));
/*    2890 */ void FUNC assign_cybs P((int usrnum , int call));
/*    2957 */ int FUNC cyb_fast P((WARSHP *ptr ));
/*    2971 */ void FUNC set_dislike P((WARUSR *wuptr , int facnum, int dislike ));

/* gecmds.c */
/*     250 */ struct cmd *FUNC gesearch P((char *ptr , struct cmd tab [], int len ));
/*     278 */ void FUNC gwar P((void ));
/*     350 */ void FUNC warnop P((void ));
/*     358 */ void FUNC cmd_gehelp P((void ));
/*     522 */ void FUNC cmd_impulse P((void ));
/*     599 */ void FUNC cmd_warp P((void ));
/*     729 */ void FUNC cmd_stop P((void ));
/*     744 */ void FUNC cmd_rotate P((void ));
/*     822 */ void FUNC cmd_flux P((void ));
/*     831 */ void FUNC cmd_orbit P((void ));
/*     909 */ void FUNC cmd_phas P((void ));
/*     992 */ void FUNC firep P((WARSHP *ptr , int usrn ));
/*    1136 */ void FUNC firehp P((WARSHP *ptr , int usrn ));
/*    1248 */ void FUNC cmd_torp P((void ));
/*    1334 */ int FUNC torp P((WARSHP *ptr , int usrn , int shpnum ));
/*    1382 */ void FUNC cmd_missl P((void ));
/*    1480 */ int FUNC misl P((WARSHP *ptr , int usrn , int shpnum , unsigned energy, unsigned eng_flu ));
/*    1525 */ int FUNC lockon P((WARSHP *ptr , int type , int ship , int usrn ));
/*    1619 */ int FUNC findshp P((char *ptr , int type ));
/*    1712 */ void FUNC zaphim P((WARSHP *ptr , int usrn ));
/*    1726 */ void FUNC cmd_decoy P((void ));
/*    1781 */ void FUNC cmd_jammer P((void ));
/*    1818 */ void FUNC jam P((WARSHP *ptr , int usrn ));
/*    1856 */ void FUNC cmd_zipper P((void ));
/*    1894 */ void FUNC zip P((WARSHP *ptr , int usrn ));
/*    1930 */ void FUNC cmd_mine P((void ));
/*    1993 */ int FUNC laymine P((WARSHP *ptr , int usrn , int timer ));
/*    2033 */ void FUNC cmd_send P((void ));
/*    2093 */ void FUNC cmd_freq P((void ));
/*    2154 */ void FUNC cmd_report P((void ));
/*    2526 */ void FUNC damstr P((int damage ));
/*    2598 */ void FUNC cmd_scan P((void ));
/*    2647 */ void FUNC scan_sh P((void ));
/*    2743 */ void FUNC scan_pl P((void ));
/*    2932 */ void FUNC scan_ra P((void ));
/*    3091 */ void FUNC scan_se P((void ));
/*    3160 */ void FUNC scan_lo P((void ));
/*    3315 */ void FUNC update_scantab P((WARSHP *ptr , int usrn ));
/*    3415 */ void FUNC pick_letter P((SCANTAB *ptr ));
/*    3736 */ void FUNC printmap P((int mask ));
/*    3777 */ unsigned FUNC coord2 P((double dcoord ));
/*    3791 */ int FUNC coord1 P((double dcoord ));
/*    3803 */ void FUNC cmd_shields P((void ));
/*    3870 */ void FUNC cmd_cloak P((void ));
/*    3954 */ void FUNC cmd_transfer P((void ));
/*    3989 */ void trans_down P((int item ));
/*    4058 */ void trans_up P((int item ));
/*    4139 */ void FUNC cmd_abandon P((void ));
/*    4181 */ void FUNC cmd_admin P((void ));
/*    4231 */ void FUNC cmd_attack P((void ));
/*    4343 */ int FUNC attack_men P((unsigned long num ));
/*    4502 */ int FUNC attack_fig P((unsigned long num ));
/*    4659 */ void FUNC call_4_help P((int send_spy_mail , int won ));
/*    4703 */ void FUNC wonplnt P((void ));
/*    4715 */ void FUNC cmd_geroster P((void ));
/*    4764 */ void FUNC cmd_planet P((void ));
/*    4804 */ void FUNC cmd_sell P((void ));
/*    4854 */ void FUNC sell P((int item ));
/*    4931 */ void FUNC cmd_buy P((void ));
/*    5015 */ void FUNC cmd_price P((void ));
/*    5054 */ void FUNC buy P((int item ));
/*    5179 */ unsigned long FUNC amt4sale P((int item ));
/*    5204 */ long FUNC price P((unsigned item , unsigned long amt ));
/*    5225 */ void FUNC cmd_maint P((void ));
/*    5317 */ void FUNC cmd_new P((void ));
/*    5546 */ void FUNC cmd_sysop P((void ));
/*    5923 */ void FUNC cmd_rename P((void ));
/*    5946 */ void FUNC cmd_destruct P((void ));
/*    5965 */ void FUNC cmd_abort P((void ));
/*    5989 */ void FUNC cmd_lock P((void ));
/*    6033 */ void FUNC cmd_navigate P((void ));
/*    6084 */ void FUNC cmd_who P((void ));
/*    6106 */ void FUNC cmd_set P((void ));
/*    6212 */ void FUNC cmd_team P((void ));
/*    6741 */ char *FUNC teamname P((WARUSR *ptr ));
/*    6758 */ void FUNC cmd_clear P((void ));
/*    6775 */ void FUNC cmd_data P((void ));
#ifdef DATACMD
/*    6891 */ void FUNC scan_data1 P((void ));
/*    6940 */ void FUNC scan_data2 P((void ));
#endif
/*    6973 */ char *FUNC gedots P((int numdots ));
/*    6992 */ void FUNC cmd_spy P((void ));
/*    7045 */ void FUNC cmd_jettison P((void ));
/*    7066 */ void FUNC jettison P((int item ));

/* gecybs.c */
/*      76 */ void FUNC cyb_init P((WARSHP *ptr , int usrn , int class ));
/*     257 */ void FUNC cyb_lives P((WARSHP *ptr , int usrn ));
/*     398 */ int FUNC notclaimed P((WARSHP *ptr , int usrn ));
/*     420 */ void FUNC cyb_annoy P((WARSHP *ptr , int usrn , int msgtype ));
/*     481 */ void FUNC cyb_msg P((WARSHP *ptr , int usrn , int msgtype ));
/*     502 */ void FUNC db_update P((WARSHP *ptr , int usrn ));
/*     539 */ void FUNC cyb_attack P((WARSHP *ptr , int usrn , WARSHP *wptr , int zothusn ));
/*     589 */ void FUNC cyb_lay_decoys P((WARSHP *ptr ));
/*     610 */ void FUNC cyb_check_damage P((WARSHP *ptr , int usrn ));
/*     641 */ void FUNC cyb_check_lockon P((WARSHP *ptr , int usrn ));
/*     875 */ void FUNC cyb_won P((WARSHP *ptr , int usrn , WARSHP *wptr ));
/*     887 */ void FUNC cyb_died P((WARSHP *ptr , int usrn , WARSHP *wptr ));
/*     903 */ void FUNC cyb_cruise P((WARSHP *ptr , int usrn , int call ));
/*     967 */ int FUNC cyb_pick_fight P((int usrn , int call ));
/*    1023 */ void FUNC cyb_check_misl P((WARSHP *ptr ));

/* gedroids.c */
/*      73 */ void FUNC droid_init P((WARSHP *ptr , int usrn , int class ));
/*     214 */ void FUNC droid_lives P((WARSHP *ptr , int usrn ));
/*     289 */ void FUNC droid_annoy P((WARSHP *ptr , int usrn ));
/*     348 */ void FUNC droid_distress P((WARSHP *ptr , int usrn ));
/*     370 */ void FUNC droid_act_1 P((WARSHP *ptr , int usrn ));
/*     434 */ void FUNC droid_act_2 P((WARSHP *ptr , int usrn ));
/*     519 */ void FUNC droid_act_3 P((WARSHP *ptr , int usrn ));
/*     604 */ void FUNC droid_act_4 P((WARSHP *ptr , int usrn ));
/*     686 */ void FUNC droid_act_5 P((WARSHP *ptr , int usrn ));
/*     838 */ void FUNC droid_act_6 P((WARSHP *ptr , int usrn ));
/*    1002 */ void FUNC droid_won P((WARSHP *ptr , int usrn));
/*    1010 */ void FUNC droid_died P((WARSHP *ptr ));
/*    1018 */ void FUNC droid_check_state P((WARSHP *ptr , int usrn ));
/*    1037 */ int FUNC notclaimed_d P((int drtype , int usrn ));
/*    1054 */ void FUNC droid_phaser P((WARSHP *ptr , int usrn , WARSHP *wptr ));
/*    1075 */ void FUNC droid_torp P((WARSHP *ptr , int usrn , WARSHP *wptr , int zothusn ));
/*    1098 */ void FUNC droid_zyg_loadout P((WARSHP *ptr ));


/* geplanet.c */
/*      73 */ void FUNC check_spy P((void ));
/*     179 */ void FUNC multiply P((int final_mult ));
/*     391 */ int FUNC getsector P((COORD *sect ));
/*     459 */ int FUNC xgetsector P((COORD *sect , int wormy ));
/*     668 */ void FUNC build_plan_1 P((int idx ));
/*     695 */ void FUNC build_plan_2 P((int idx ));
/*     723 */ void FUNC build_other P((int idx ));
/*     752 */ void FUNC build_worm P((int idx ));
/*     798 */ int FUNC getplanet P((COORD *sect , int plnt ));
/*     819 */ int FUNC neutral P((COORD *coord ));
/*     827 */ void FUNC update_plan_1 P((void ));
/*     837 */ void FUNC update_plan_2 P((void ));
/*     853 */ void FUNC update_plan_3 P((void ));

/* gelib.c */
/*      65 */ unsigned smallest P((unsigned a1 , unsigned a2 ));
/*      84 */ double rndm P((double mod ));
/*      94 */ unsigned int gernd P((void ));
/*     134 */ double cbearing P((COORD *ptr1 , COORD *ptr2 , double heading ));
/*     141 */ double cdistance P((COORD *ptr1 , COORD *ptr2 ));
/*     156 */ double vector P((COORD *ptr1 , COORD *ptr2 ));
/*     199 */ double angleb P((COORD *ptr1 , COORD *ptr2 ));
/*     229 */ double anglec P((COORD *ptr1 , COORD *ptr2 ));
/*     259 */ double normal P((double angle ));
/*     280 */ double degtorad P((double value ));
/*     290 */ double radtodeg P((double value ));
/*     300 */ double absol P((double value ));

/* gescan.c */
void FUNC set_map_color P((int color ));
void FUNC print_ship_letter P((int othusn , char letter ));
void FUNC print_ship_data P((long dist , int bearing , int heading , double speed ));
void FUNC print_ship_name P((int othusn ));
int FUNC print_planet_line P((int shp ));
void FUNC print_map_header P((int maptype ));
void FUNC print_map_row P((int i , int maptype ));
void FUNC print_ship_pair P((SCANTAB *sptr , int left , int right , long dist_filter ));
int FUNC print_range_line P((SCANTAB *sptr , int shp , int *ff , long dist_filter ));
int FUNC print_fullrange_line P((SCANTAB *sptr , int shp , long dist_filter ));
void FUNC print_range_summary P((SCANTAB *sptr, int shp, long dist_filter, int maptype));
void FUNC print_planet_summary P((int shp));

#undef P
