/****************************************************************************************/
/*                                                                                      */
/* PROGETTO : C534/C538 Carel IR33/Power Compact                                        */
/*                                                                                      */
/* MODULO   : paramdes.h                                                                */
/* DATA     : 27/03/2013 - 11.16.28                                                     */
/*                                                                                      */
/* AUTORE   : Lorenzo Melato                                                            */
/* VERSIONE : 1.0                                                                       */
/* RELEASE  :                                                                           */
/*                                                                                      */
/****************************************************************************************/


#define MAX_IDX_PAR       242                 // numero massimo parametri


enum {
                                        // Carel Protocol |    Modbus Protocol     - Description
  SPV_VIS_SONDA_DEF_1,                  // Analogica   1  | Register   1, adr.   0 - d/1: Visualizzazione sonda defrost
  SPV_VIS_SONDA_DEF_2,                  // Analogica   2  | Register   2, adr.   1 - d/2: Visualizzazione sonda defrost 2
  SPV_S_SONDA_VIRTUALE,                 // Analogica   3  | Register   3, adr.   2 - s_prre: Sonda virtuale
  SPV_S_SONDA_1,                        // Analogica   4  | Register   4, adr.   3 - s_pr1: Misura sonda 1
  SPV_S_SONDA_2,                        // Analogica   5  | Register   5, adr.   4 - s_pr2: Misura sonda 2
  SPV_S_SONDA_3,                        // Analogica   6  | Register   6, adr.   5 - s_pr3: Misura sonda 3
  SPV_S_SONDA_4,                        // Analogica   7  | Register   7, adr.   6 - s_pr4: Misura sonda 4
  SPV_S_SONDA_5,                        // Analogica   8  | Register   8, adr.   7 - s_pr5: Misura sonda 5
  SPV_S_A_FREE_1,                       // Analogica   9  | Register   9, adr.   8 - s_AFree1: Libera per usi futuri
  SPV_S_A_FREE_2,                       // Analogica  10  | Register  10, adr.   9 - s_AFree2: Libera per usi futuri
  SPV_CALIB_SONDA1,                     // Analogica  11  | Register  11, adr.  10 - /c1: Calibrazione sonda 1
  SPV_CALIB_SONDA2,                     // Analogica  12  | Register  12, adr.  11 - /c2: Calibrazione sonda 2
  SPV_CALIB_SONDA3,                     // Analogica  13  | Register  13, adr.  12 - /c3: Calibrazione sonda 3
  SPV_CALIB_SONDA4,                     // Analogica  14  | Register  14, adr.  13 - /c4: Calibrazione sonda 4
  SPV_CALIB_SONDA5,                     // Analogica  15  | Register  15, adr.  14 - /c5: Calibrazione sonda 5
  SPV_SET_POINT_TEMPERATURA,            // Analogica  16  | Register  16, adr.  15 - St: Set point
  SPV_DIFFERENZIALE_REGOLATORE,         // Analogica  17  | Register  17, adr.  16 - rd: Delta Regolatore
  SPV_SET_MIN,                          // Analogica  18  | Register  18, adr.  17 - r1: SET minimo ammesso
  SPV_SET_MAX,                          // Analogica  19  | Register  19, adr.  18 - r2: SET massimo ammesso
  SPV_SET_POINT_NOTTURNO,               // Analogica  20  | Register  20, adr.  19 - r4: Variazione automatica Set point notturno
  SPV_MAX_TEMP_LETTA,                   // Analogica  21  | Register  21, adr.  20 - rH: Massima temperatura letta
  SPV_MIN_TEMP_LETTA,                   // Analogica  22  | Register  22, adr.  21 - rL: Minima temperatura letta
  SPV_TEMP_FINE_DEFROST_E1,             // Analogica  23  | Register  23, adr.  22 - dt1: Temperatura di fine defrost evap.
  SPV_TEMP_FINE_DEFROST_E2,             // Analogica  24  | Register  24, adr.  23 - dt2: Temperatura di fine defrost evap. aux
  SPV_TEMP_RUNNING_TIME,                // Analogica  25  | Register  25, adr.  24 - d11: Soglia di temperatura di running time
  SPV_DIFFERENZIALE_ALM_FAN,            // Analogica  26  | Register  26, adr.  25 - A0: Differenziale allarmi e ventole
  SPV_ALM_BASSA_TEMP,                   // Analogica  27  | Register  27, adr.  26 - AL: Soglia di allarme di bassa temperatura
  SPV_ALM_ALTA_TEMP,                    // Analogica  28  | Register  28, adr.  27 - AH: Soglia di allarme di alta temperatura
  SPV_ALM_ALTA_TEMP_COND,               // Analogica  29  | Register  29, adr.  28 - Ac: Allarme alta temperatura condensatore
  SPV_DIFFERENZIALE_ALM_AE,             // Analogica  30  | Register  30, adr.  29 - AE: Differenziale allarme alta temp. cond.
  SPV_TEMP_ON_FAN,                      // Analogica  31  | Register  31, adr.  30 - F1: Temperatura accensione ventola
  SPV_TEMP_OFF_FAN_COND,                // Analogica  32  | Register  32, adr.  31 - F4: Temperatura spegnimento fan condensatore
  SPV_DIFF_FAN_COND,                    // Analogica  33  | Register  33, adr.  32 - F5: Differenziale ventola condensatore
  SPV_ZONA_NEUTRA,                      // Analogica  34  | Register  34, adr.  33 - rn: Zona neutra
  SPV_DIFFERENZIALE_ZONA_NEUTRA,        // Analogica  35  | Register  35, adr.  34 - rr: Delta regolatore reverse con zona neutra
  SPV_SOGLIA_ALM_ANTIFREEZE,            // Analogica  36  | Register  36, adr.  35 - ALF: Soglia di allarme antifreeze
  SPV_ANTISWEAT_OFFSET,                 // Analogica  37  | Register  37, adr.  36 - Hdh: Offset anti-sweat heater
  SPV_TEMP_FINE_DEFROST_E3,             // Analogica  38  | Register  38, adr.  37 - dt3: Temperatura di fine defrost terza sonda
  SPV_RTC_ANNO,                         // Intera      1  | Register 101, adr. 100 - tc_y__: tc_y__
  SPV_RTC_MESE,                         // Intera      2  | Register 102, adr. 101 - tc_M__: tc_M__
  SPV_RTC_GIORNO_MESE,                  // Intera      3  | Register 103, adr. 102 - tc_d__: tc_d__
  SPV_RTC_GIORNO_SETTIM,                // Intera      4  | Register 104, adr. 103 - tc_u__: tc_u__
  SPV_RTC_ORA,                          // Intera      5  | Register 105, adr. 104 - tc_h__: tc_h__
  SPV_RTC_MINUTO,                       // Intera      6  | Register 106, adr. 105 - tc_m__: tc_m__
  SPV_S_DI_VARIATION_CALC,              // Intera      7  | Register 107, adr. 106 - s_vdI: Intervallo di dI calcolato
  SPV_S_LAN_REQUEST,                    // Intera      8  | Register 108, adr. 107 - s_QDf: Richieste al sincronizzatore
  SPV_S_LAN_ALM_ENB_FLAG,               // Intera      9  | Register 109, adr. 108 - s_WSA: Flag allarmi, abilitazioni sincronizzatore
  SPV_TIPO_SONDA,                       // Intera     10  | Register 110, adr. 109 - ntcptc: Tipo di sonda ntc/ptc
  SPV_TIPO_CONTROLLO,                   // Intera     11  | Register 111, adr. 110 - ctrl: Tipo di controllo
  SPV_MODELLO_MACCHINA,                 // Intera     12  | Register 112, adr. 111 - model: Modello macchina
  SPV_PRESENZA_RTC,                     // Intera     13  | Register 113, adr. 112 - pres_rtc: Indicatore rtc assente/presente
  SPV_PASSWORD,                         // Intera     14  | Register 114, adr. 113 - Pw: password
  SPV_STABILITA_MISURA,                 // Intera     15  | Register 115, adr. 114 - /2: Stabilità misura
  SPV_RALL_VIS_SONDA,                   // Intera     16  | Register 116, adr. 115 - /3: Rallentamento visual. Sonda
  SPV_SONDA_VIRTUALE,                   // Intera     17  | Register 117, adr. 116 - /4: Sonda virtuale
  SPV_INTERNAL_TERMINAL,                // Intera     18  | Register 118, adr. 117 - /tI: Visual. su terminale interno
  SPV_EXTERNAL_TERMINAL,                // Intera     19  | Register 119, adr. 118 - /tE: Visual. su terminale esterno
  SPV_TIPO_NTC,                         // Intera     20  | Register 120, adr. 119 - /P: Selezione tipo di sonda NTC
  SPV_CONFIG_SONDA2,                    // Intera     21  | Register 121, adr. 120 - /A2: Configurazione sonda 2
  SPV_CONFIG_SONDA3,                    // Intera     22  | Register 122, adr. 121 - /A3: Configurazione sonda 3
  SPV_CONFIG_SONDA4,                    // Intera     23  | Register 123, adr. 122 - /A4: Configurazione sonda 4
  SPV_CONFIG_SONDA5,                    // Intera     24  | Register 124, adr. 123 - /A5: Configurazione sonda 5
  SPV_MODALITA_FUNZIONAMENTO,           // Intera     25  | Register 125, adr. 124 - r3: Modalità di funzionamento
  SPV_INTEVALLO_MONITOR_TEMP,           // Intera     26  | Register 126, adr. 125 - rt: Intervallo monitoraggio temperatura
  SPV_RITARDO_START_COMP_FAN,           // Intera     27  | Register 127, adr. 126 - c0: Ritardo Start Comp. e Fan all’accensione
  SPV_TEMPO_MINIMO_TRA_START,           // Intera     28  | Register 128, adr. 127 - c1: Tempo minimo tra accensioni successive
  SPV_TEMPO_MINIMO_DI_OFF,              // Intera     29  | Register 129, adr. 128 - c2: Tempo minimo di Off del compressore
  SPV_TEMPO_MINIMO_DI_ON,               // Intera     30  | Register 130, adr. 129 - c3: Tempo minimo di On del compressore
  SPV_DUTY_SETTING,                     // Intera     31  | Register 131, adr. 130 - c4: Duty setting
  SPV_DURATA_CICLO_CONT,                // Intera     32  | Register 132, adr. 131 - cc: Durata ciclo continuo
  SPV_ESCL_ALM_DOPO_CC,                 // Intera     33  | Register 133, adr. 132 - c6: Esclusione allarme dopo ciclo continuo
  SPV_TEMPO_MAX_PUMP_DOWN,              // Intera     34  | Register 134, adr. 133 - c7: Tempo massimo di Pump-Down
  SPV_RIT_START_COMP_DOPO_PD,           // Intera     35  | Register 135, adr. 134 - c8: Ritardo start comp. dopo apert. valvola PD
  SPV_DELAY_2_COMP,                     // Intera     36  | Register 136, adr. 135 - c11: Ritardo secondo compressore
  SPV_TIPO_DEFROST,                     // Intera     37  | Register 137, adr. 136 - d0: Tipo di defrost
  SPV_INTERVALLO_DEFROST,               // Intera     38  | Register 138, adr. 137 - dI: Intervallo tra i defrost
  SPV_DURATA_MAX_DEFROST_E1,            // Intera     39  | Register 139, adr. 138 - dP1: Durata massima defrost evap.
  SPV_DURATA_MAX_DEFROST_E2,            // Intera     40  | Register 140, adr. 139 - dP2: Durata massima defrost evap. aux
  SPV_RITARDO_INS_DEFROST,              // Intera     41  | Register 141, adr. 140 - d3: Ritardo inserimento defrost
  SPV_RITARDO_DEFROST_STARTUP,          // Intera     42  | Register 142, adr. 141 - d5: Ritardo defrost allo start up
  SPV_BLOCCO_DISPLAY_DEFROST,           // Intera     43  | Register 143, adr. 142 - d6: Blocco display durante il defrost
  SPV_TEMPO_GOCCIOLAMENTO,              // Intera     44  | Register 144, adr. 143 - dd: Tempo di gocciolamento dopo il defrost
  SPV_ESCL_ALM_DOPO_DEFROST,            // Intera     45  | Register 145, adr. 144 - d8: Esclusione allarmi dopo il defrost
  SPV_RUNNING_TIME_COMP,                // Intera     46  | Register 146, adr. 145 - d10: Running time del compressore
  SPV_DEFROST_AVANZATI,                 // Intera     47  | Register 147, adr. 146 - d12: Defrost avanzati
  SPV_DURATA_NOMINALE_DEFROST,          // Intera     48  | Register 148, adr. 147 - dn: Durata nominale defrost
  SPV_PROP_DURATA_DEFROST,              // Intera     49  | Register 149, adr. 148 - dH: Fattore proporzionale variaz. di ‘dI’
  SPV_RITARDO_ALM_AL_AH,                // Intera     50  | Register 150, adr. 149 - Ad: Ritardo allarme bassa e alta temperatura
  SPV_CONFIG_DIGIN_1,                   // Intera     51  | Register 151, adr. 150 - A4: Configurazione ingresso digitale 1
  SPV_CONFIG_DIGIN_2,                   // Intera     52  | Register 152, adr. 151 - A5: Configurazione ingresso digitale 2
  SPV_STOP_DEFROST_EXT_ALM,             // Intera     53  | Register 153, adr. 152 - A6: Blocco compressore da allarme esterno
  SPV_RITARDO_EXT_ALM,                  // Intera     54  | Register 154, adr. 153 - A7: Ritardo rilevazione allarme esterno
  SPV_CONFIG_DIGIN_3,                   // Intera     55  | Register 155, adr. 154 - A9: Configurazione ingresso digitale 3
  SPV_RIT_ALM_ALTA_TEMP_COND,           // Intera     56  | Register 156, adr. 155 - Acd: Ritardo allarme alta temperatura condens.
  SPV_TEMPO_SENSORE_DI_LUCE,            // Intera     57  | Register 157, adr. 156 - AF: Tempo spegnimento con sensore di luce
  SPV_MANAG_FAN,                        // Intera     58  | Register 158, adr. 157 - F0: Gestione ventola
  SPV_STOP_FAN_DOPO_GOCCIOL,            // Intera     59  | Register 159, adr. 158 - Fd: Spegnimento Fan dopo gocciolamento
  SPV_INDIRIZZO_SERIALE,                // Intera     60  | Register 160, adr. 159 - H0: Indirizzo seriale
  SPV_CONFIG_RELE4,                     // Intera     61  | Register 161, adr. 160 - H1: Funzionalità Relè 4
  SPV_DISABLE_KEY_IR,                   // Intera     62  | Register 162, adr. 161 - H2: Disabilitazione tastiera/Ir
  SPV_CODICE_ABILIT_RC,                 // Intera     63  | Register 163, adr. 162 - H3: Codice abilitazione telecomando
  SPV_CONFIG_RELE5,                     // Intera     64  | Register 164, adr. 163 - H5: Funzionalità Relè 5
  SPV_BLOCCO_TASTI,                     // Intera     65  | Register 165, adr. 164 - H6: Blocco tasti
  SPV_PROFILO_STAMPA,                   // Intera     66  | Register 166, adr. 165 - HPr: Profilo di stampa
  SPV_NUMERO_HA,                        // Intera     67  | Register 167, adr. 166 - HAn: Numero di eventi HA intervenuti
  SPV_NUMERO_HF,                        // Intera     68  | Register 168, adr. 167 - HFn: Numero di eventi HF intervenuti
  SPV_RITARDO_ALM_HACCP,                // Intera     69  | Register 169, adr. 168 - Htd: Ritardo allarme HACCP
  SPV_HA_ANNO,                          // Intera     70  | Register 170, adr. 169 - HA_y__: HA_y__
  SPV_HA_MESE,                          // Intera     71  | Register 171, adr. 170 - HA_M__: HA_M__
  SPV_HA_GIORNO,                        // Intera     72  | Register 172, adr. 171 - HA_d__: HA_d__
  SPV_HA_ORA,                           // Intera     73  | Register 173, adr. 172 - HA_h__: HA_h__
  SPV_HA_MINUTO,                        // Intera     74  | Register 174, adr. 173 - HA_m__: HA_m__
  SPV_HA_DURATA,                        // Intera     75  | Register 175, adr. 174 - HA_t__: HA_t__
  SPV_HA1_ANNO,                         // Intera     76  | Register 176, adr. 175 - HA1_y__: HA1_y__
  SPV_HA1_MESE,                         // Intera     77  | Register 177, adr. 176 - HA1_M__: HA1_M__
  SPV_HA1_GIORNO,                       // Intera     78  | Register 178, adr. 177 - HA1_d__: HA1_d__
  SPV_HA1_ORA,                          // Intera     79  | Register 179, adr. 178 - HA1_h__: HA1_h__
  SPV_HA1_MINUTO,                       // Intera     80  | Register 180, adr. 179 - HA1_m__: HA1_m__
  SPV_HA1_DURATA,                       // Intera     81  | Register 181, adr. 180 - HA1_t__: HA1_t__
  SPV_HA2_ANNO,                         // Intera     82  | Register 182, adr. 181 - HA2_y__: HA2_y__
  SPV_HA2_MESE,                         // Intera     83  | Register 183, adr. 182 - HA2_M__: HA2_M__
  SPV_HA2_GIORNO,                       // Intera     84  | Register 184, adr. 183 - HA2_d__: HA2_d__
  SPV_HA2_ORA,                          // Intera     85  | Register 185, adr. 184 - HA2_h__: HA2_h__
  SPV_HA2_MINUTO,                       // Intera     86  | Register 186, adr. 185 - HA2_m__: HA2_m__
  SPV_HA2_DURATA,                       // Intera     87  | Register 187, adr. 186 - HA2_t__: HA2_t__
  SPV_HF_ANNO,                          // Intera     88  | Register 188, adr. 187 - HF_y__: HF_y__
  SPV_HF_MESE,                          // Intera     89  | Register 189, adr. 188 - HF_M__: HF_M__
  SPV_HF_GIORNO,                        // Intera     90  | Register 190, adr. 189 - HF_d__: HF_d__
  SPV_HF_ORA,                           // Intera     91  | Register 191, adr. 190 - HF_h__: HF_h__
  SPV_HF_MINUTO,                        // Intera     92  | Register 192, adr. 191 - HF_m__: HF_m__
  SPV_HF_DURATA,                        // Intera     93  | Register 193, adr. 192 - HF_t__: HF_t__
  SPV_HF1_ANNO,                         // Intera     94  | Register 194, adr. 193 - HF1_y__: HF1_y__
  SPV_HF1_MESE,                         // Intera     95  | Register 195, adr. 194 - HF1_M__: HF1_M__
  SPV_HF1_GIORNO,                       // Intera     96  | Register 196, adr. 195 - HF1_d__: HF1_d__
  SPV_HF1_ORA,                          // Intera     97  | Register 197, adr. 196 - HF1_h__: HF1_h__
  SPV_HF1_MINUTO,                       // Intera     98  | Register 198, adr. 197 - HF1_m__: HF1_m__
  SPV_HF1_DURATA,                       // Intera     99  | Register 199, adr. 198 - HF1_t__: HF1_t__
  SPV_HF2_ANNO,                         // Intera    100  | Register 200, adr. 199 - HF2_y__: HF2_y__
  SPV_HF2_MESE,                         // Intera    101  | Register 201, adr. 200 - HF2_M__: HF2_M__
  SPV_HF2_GIORNO,                       // Intera    102  | Register 202, adr. 201 - HF2_d__: HF2_d__
  SPV_HF2_ORA,                          // Intera    103  | Register 203, adr. 202 - HF2_h__: HF2_h__
  SPV_HF2_MINUTO,                       // Intera    104  | Register 204, adr. 203 - HF2_m__: HF2_m__
  SPV_HF2_DURATA,                       // Intera    105  | Register 205, adr. 204 - HF2_t__: HF2_t__
  SPV_FASCIA_ORARIA_1_GIORNO,           // Intera    106  | Register 206, adr. 205 - td1_d__: td1_d__
  SPV_FASCIA_ORARIA_1_ORA,              // Intera    107  | Register 207, adr. 206 - td1_h__: td1_h__
  SPV_FASCIA_ORARIA_1_MINUTO,           // Intera    108  | Register 208, adr. 207 - td1_m__: td1_m__
  SPV_FASCIA_ORARIA_2_GIORNO,           // Intera    109  | Register 209, adr. 208 - td2_d__: td2_d__
  SPV_FASCIA_ORARIA_2_ORA,              // Intera    110  | Register 210, adr. 209 - td2_h__: td2_h__
  SPV_FASCIA_ORARIA_2_MINUTO,           // Intera    111  | Register 211, adr. 210 - td2_m__: td2_m__
  SPV_FASCIA_ORARIA_3_GIORNO,           // Intera    112  | Register 212, adr. 211 - td3_d__: td3_d__
  SPV_FASCIA_ORARIA_3_ORA,              // Intera    113  | Register 213, adr. 212 - td3_h__: td3_h__
  SPV_FASCIA_ORARIA_3_MINUTO,           // Intera    114  | Register 214, adr. 213 - td3_m__: td3_m__
  SPV_FASCIA_ORARIA_4_GIORNO,           // Intera    115  | Register 215, adr. 214 - td4_d__: td4_d__
  SPV_FASCIA_ORARIA_4_ORA,              // Intera    116  | Register 216, adr. 215 - td4_h__: td4_h__
  SPV_FASCIA_ORARIA_4_MINUTO,           // Intera    117  | Register 217, adr. 216 - td4_m__: td4_m__
  SPV_FASCIA_ORARIA_5_GIORNO,           // Intera    118  | Register 218, adr. 217 - td5_d__: td5_d__
  SPV_FASCIA_ORARIA_5_ORA,              // Intera    119  | Register 219, adr. 218 - td5_h__: td5_h__
  SPV_FASCIA_ORARIA_5_MINUTO,           // Intera    120  | Register 220, adr. 219 - td5_m__: td5_m__
  SPV_FASCIA_ORARIA_6_GIORNO,           // Intera    121  | Register 221, adr. 220 - td6_d__: td6_d__
  SPV_FASCIA_ORARIA_6_ORA,              // Intera    122  | Register 222, adr. 221 - td6_h__: td6_h__
  SPV_FASCIA_ORARIA_6_MINUTO,           // Intera    123  | Register 223, adr. 222 - td6_m__: td6_m__
  SPV_FASCIA_ORARIA_7_GIORNO,           // Intera    124  | Register 224, adr. 223 - td7_d__: td7_d__
  SPV_FASCIA_ORARIA_7_ORA,              // Intera    125  | Register 225, adr. 224 - td7_h__: td7_h__
  SPV_FASCIA_ORARIA_7_MINUTO,           // Intera    126  | Register 226, adr. 225 - td7_m__: td7_m__
  SPV_FASCIA_ORARIA_8_GIORNO,           // Intera    127  | Register 227, adr. 226 - td8_d__: td8_d__
  SPV_FASCIA_ORARIA_8_ORA,              // Intera    128  | Register 228, adr. 227 - td8_h__: td8_h__
  SPV_FASCIA_ORARIA_8_MINUTO,           // Intera    129  | Register 229, adr. 228 - td8_m__: td8_m__
  SPV_FASCIA_ORARIA_ON_GIORNO,          // Intera    130  | Register 230, adr. 229 - ton_d__: ton_d__
  SPV_FASCIA_ORARIA_ON_ORA,             // Intera    131  | Register 231, adr. 230 - ton_h__: ton_h__
  SPV_FASCIA_ORARIA_ON_MINUTO,          // Intera    132  | Register 232, adr. 231 - ton_m__: ton_m__
  SPV_FASCIA_ORARIA_OFF_GIORNO,         // Intera    133  | Register 233, adr. 232 - toff_d__: toff_d__
  SPV_FASCIA_ORARIA_OFF_ORA,            // Intera    134  | Register 234, adr. 233 - toff_h__: toff_h__
  SPV_FASCIA_ORARIA_OFF_MINUTO,         // Intera    135  | Register 235, adr. 234 - toff_m__: toff_m__
  SPV_RITARDO_ALM_ANTIFREEZE,           // Intera    136  | Register 236, adr. 235 - AdF: Ritardo allarme Anti freeze 
  SPV_NUM_DEFPAR_PRES,                  // Intera    137  | Register 237, adr. 236 - Hdn: Numero set parametri di default disponibili
  SPV_LAN_MST_SLV_SEL,                  // Intera    138  | Register 238, adr. 237 - In: Seleziona indirizzo Master Slave
  SPV_RITARDO_ALM_PORTA,                // Intera    139  | Register 239, adr. 238 - d8d: Ritardo allarme porta aperta per troppo tempo
  SPV_S_RELE_1,                         // Digitale    1  | Coil   1, adr.   0     - s_rl1: Stato Rele compressore
  SPV_S_RELE_2,                         // Digitale    2  | Coil   2, adr.   1     - s_rl2: Stato Rele defrost
  SPV_S_RELE_3,                         // Digitale    3  | Coil   3, adr.   2     - s_rl3: Stato Rele ventole
  SPV_S_RELE_4,                         // Digitale    4  | Coil   4, adr.   3     - s_rl4: Stato Rele ausiliario 1
  SPV_S_RELE_5,                         // Digitale    5  | Coil   5, adr.   4     - s_rl5: Stato Rele ausiliario 2
  SPV_S_DIGIN_1,                        // Digitale    6  | Coil   6, adr.   5     - s_din1: Stato ingresso digitale 1
  SPV_S_DIGIN_2,                        // Digitale    7  | Coil   7, adr.   6     - s_din2: Stato ingresso digitale 2
  SPV_S_DIGIN_3,                        // Digitale    8  | Coil   8, adr.   7     - s_din3: Stato ingresso digitale 3
  SPV_S_ALM_SONDA_VIRTUALE,             // Digitale    9  | Coil   9, adr.   8     - s_re: Allarme sonda virtuale guasta
  SPV_S_ALM_SONDA_1,                    // Digitale   10  | Coil  10, adr.   9     - s_pre1: Allarme sonda 1 guasta
  SPV_S_ALM_SONDA_2,                    // Digitale   11  | Coil  11, adr.  10     - s_pre2: Allarme sonda 2 guasta
  SPV_S_ALM_SONDA_3,                    // Digitale   12  | Coil  12, adr.  11     - s_pre3: Allarme sonda 3 guasta
  SPV_S_ALM_SONDA_4,                    // Digitale   13  | Coil  13, adr.  12     - s_pre4: Allarme sonda 4 guasta
  SPV_S_ALM_SONDA_5,                    // Digitale   14  | Coil  14, adr.  13     - s_pre5: Allarme sonda 5 guasta
  SPV_S_ALM_BASSA_TEMP,                 // Digitale   15  | Coil  15, adr.  14     - s_LO: Allarme di bassa temperatura
  SPV_S_ALM_ALTA_TEMP,                  // Digitale   16  | Coil  16, adr.  15     - s_HI: Allarme di alta temperatura
  SPV_S_ALM_IMMEDIATO_ESTERNO,          // Digitale   17  | Coil  17, adr.  16     - s_IA: Allarme immediato esterno
  SPV_S_ALM_RITARDATO_ESTERNO,          // Digitale   18  | Coil  18, adr.  17     - s_dA: Allarme ritardato esterno
  SPV_S_ALM_TIMEOUT_DEF_EVAP1,          // Digitale   19  | Coil  19, adr.  18     - s_Ed1: Allarme timeout defrost evaporatore 1
  SPV_S_ALM_TIMEOUT_DEF_EVAP2,          // Digitale   20  | Coil  20, adr.  19     - s_Ed2: Allarme timeout defrost evaporatore 2
  SPV_S_ALM_TIMEOUT_PUMPDOWN,           // Digitale   21  | Coil  21, adr.  20     - s_Pd: Allarme timeout Pump Down
  SPV_S_ALM_BASSA_PRESSIONE,            // Digitale   22  | Coil  22, adr.  21     - s_LP: Allarme bassa pressione
  SPV_S_ALM_PRE_ALTA_TEMP_COND,         // Digitale   23  | Coil  23, adr.  22     - s_cht: Preallarme alta temperatura condensatore
  SPV_S_ALM_ALTA_TEMP_COND,             // Digitale   24  | Coil  24, adr.  23     - s_CHT: Allarme alta temperatura condensatore
  SPV_S_ALM_TIMEOUT_PORTA,              // Digitale   25  | Coil  25, adr.  24     - s_dor: Allarme porta aperta per troppo tempo
  SPV_S_ALM_RTC_GUASTO,                 // Digitale   26  | Coil  26, adr.  25     - s_Etc: Allarme RTC guasto
  SPV_S_ALM_EEPROM_PAR_MACC,            // Digitale   27  | Coil  27, adr.  26     - s_EE: Allarme Eeprom parametri macchina
  SPV_S_ALM_EEPROM_PAR_FUNZ,            // Digitale   28  | Coil  28, adr.  27     - s_EF: Allarme Eeprom parametri funzionamento
  SPV_S_ALM_HA,                         // Digitale   29  | Coil  29, adr.  28     - s_HA: Allarme HACCP di tipo HA
  SPV_S_ALM_HF,                         // Digitale   30  | Coil  30, adr.  29     - s_HF: Allarme HACCP di tipo HF
  SPV_S_STATO_DEFROST,                  // Digitale   31  | Coil  31, adr.  30     - s_DEF: Stato di defrost
  SPV_S_ALM_AUTOSTART_PD,               // Digitale   32  | Coil  32, adr.  31     - s_AtS: Allarme autostart Pump Down
  SPV_S_STATO_PRINT,                    // Digitale   33  | Coil  33, adr.  32     - s_Prt: Stato stampa report
  SPV_S_COMANDO_DEFROST,                // Digitale   34  | Coil  34, adr.  33     - s_cDEF: Comando richiesta di defrost
  SPV_S_STATO_CICLO_CONTINUO,           // Digitale   35  | Coil  35, adr.  34     - s_CC: Stato di ciclo continuo
  SPV_S_COMANDO_CICLO_CONTINUO,         // Digitale   36  | Coil  36, adr.  35     - s_cCC: Comando richiesta di ciclo continuo
  SPV_S_STATO_PORTA,                    // Digitale   37  | Coil  37, adr.  36     - s_doors: Stato porta
  SPV_S_ALM_ANTIFREEZE,                 // Digitale   38  | Coil  38, adr.  37     - s_Afr: Allarme Anti freeze
  SPV_S_SET_NIGHT_RTC,                  // Digitale   39  | Coil  39, adr.  38     - s_NigSet: Setpoint variato da RTC
  SPV_UNITA_MISURA,                     // Digitale   40  | Coil  40, adr.  39     - /5: Selezione°C o°F
  SPV_DECIMAL_POINT,                    // Digitale   41  | Coil  41, adr.  40     - /6: Punto decimale
  SPV_ENABLE_MONITOR_TEMP,              // Digitale   42  | Coil  42, adr.  41     - r5: Abilitazione monitoraggio temperatura
  SPV_ENABLE_AUTOSTART_PD,              // Digitale   43  | Coil  43, adr.  42     - c9: Abil. funz. di autostart con funz.in PD
  SPV_PD_TEMPO_PRESS,                   // Digitale   44  | Coil  44, adr.  43     - c10: Selez. Pump-Down a tempo o pressione
  SPV_DEFROST_STARTUP,                  // Digitale   45  | Coil  45, adr.  44     - d4: Abilitazione defrost allo start up
  SPV_PRIORITA_DEFROST_COMP,            // Digitale   46  | Coil  46, adr.  45     - d9: Priorità defrost su protezioni compressore
  SPV_BASE_TEMPI_DEFROST,               // Digitale   47  | Coil  47, adr.  46     - dC: Base dei tempi per defrost
  SPV_TIPO_SOGLIA_AL_AH,                // Digitale   48  | Coil  48, adr.  47     - A1: Tipo di soglia ‘AL’ e ‘AH’
  SPV_ENABLE_ALM_ED1_ED2,               // Digitale   49  | Coil  49, adr.  48     - A8: Abilitazione allarmi ‘Ed1’ e ‘Ed2’
  SPV_DOOR_MODE,                        // Digitale   50  | Coil  50, adr.  49     - Ado: Configurazione modalità luce porta
  SPV_TEMP_OFF_FAN,                     // Digitale   51  | Coil  51, adr.  50     - F2: Ventola Off con compressore Off
  SPV_FAN_IN_DEFROST,                   // Digitale   52  | Coil  52, adr.  51     - F3: Ventole in sbrinamento
  SPV_DISABLE_BUZZER,                   // Digitale   53  | Coil  53, adr.  52     - H4: Disabilitazione buzzer
  SPV_SELEZIONE_TASTIERA,               // Digitale   54  | Coil  54, adr.  53     - H7: Selezione tastiera
  SPV_ALM_HACCP,                        // Digitale   55  | Coil  55, adr.  54     - Ht0: Allarme HACCP
  SPV_S_COMANDO_BASSA_UMIDITA,          // Digitale   56  | Coil  56, adr.  55     - s_cLrH: Comando bassa umidità relativa
  SPV_S_COMANDO_AUX,                    // Digitale   57  | Coil  57, adr.  56     - s_cAUX: Comando attivazione AUX
  SPV_S_COMANDO_LUCE,                   // Digitale   58  | Coil  58, adr.  57     - s_cLUX: Comando attivazione LUCE
  SPV_S_COMANDO_ON_OFF,                 // Digitale   59  | Coil  59, adr.  58     - s_cONOFF: Comando di ON/OFF del controllo
  SPV_RTC_LUCE_AUX,                     // Digitale   60  | Coil  60, adr.  59     - H8: Comando luce o aux da RTC
  SPV_RTC_SET_CHANGE,                   // Digitale   61  | Coil  61, adr.  60     - H9: Abilitazione variazione setpoint da RTC.
  SPV_LAN_ENABLE_LIGHT_RELE,            // Digitale   62  | Coil  62, adr.  61     - HrL: Abilita propagazione Rele Luce del Master
  SPV_LAN_ENABLE_AUX_RELE,              // Digitale   63  | Coil  63, adr.  62     - HrA: Abilita propagazione Rele Aux del Master
  SPV_LAN_ENABLE_ALM_PROP,              // Digitale   64  | Coil  64, adr.  63     - HSA: Abilita propagazione Allarmi dispositivi
  SPV_BASE_TEMPI_RITALM,                // Digitale   65  | Coil  65, adr.  64     - dC1: Base dei tempi per ritardo allarmi
  LAST_IDX_PAR
};




#ifndef STRUTTURA_PARAMETRI
#define STRUTTURA_PARAMETRI
/* definizione struttura parametri interni */
typedef struct
{
  WORD index;
  short max_value;
  short min_value;
  BYTE flags1;
  BYTE flags2;
} param_struct;
#endif // STRUTTURA_PARAMETRI




#define START_MACHINE_INDEX        0

#define START_PAR_INDEX            17

#define SIZE_ARRAY                 181

#define SIZE_LIVELLI_VISIBILITA    32

#define SIZE_ANL_SPV_STAT          20
#define SIZE_INT_SPV_STAT          11
#define SIZE_DIG_SPV_STAT          5


#define MAX_ANL_SPV_STAT     10
#define MAX_ANL_SPV_PARAM    28
#define MAX_INT_SPV_STAT     9
#define MAX_INT_SPV_PARAM    130
#define MAX_DIG_SPV_STAT     39
#define MAX_DIG_SPV_PARAM    26


// Modbus Registers
#define MODBUS_ANL_SPV_RANGE_LO    0
#define MODBUS_ANL_SPV_RANGE_HI    37
#define MODBUS_INT_SPV_RANGE_LO    100
#define MODBUS_INT_SPV_RANGE_HI    238

// Modbus Coils
#define MODBUS_DIG_SPV_RANGE_LO    0
#define MODBUS_DIG_SPV_RANGE_HI    64




/* Definizioni flags 1

/* bit 0-1 */
#define I_TYPE               0x00    // parametro/variabile intera
#define A_TYPE               0x01    // parametro/variabile analogica
#define D_TYPE               0x02    // parametro/variabile digitale

/* bit 2 */
#define PAR_VAR              0x00    // variabile parametro
#define STATUS_VAR           0x04    // variabile di stato

/* bit 3 */
#define R_TYPE               0x00    // parametro tipo read only
#define R_W_TYPE             0x08    // parametro tipo read/write

/* bit 4-6 */
#define POS0                 0x00    // posizione 0 all'interno della bitmap
#define POS1                 0x10    // posizione 1 all'interno della bitmap
#define POS2                 0x20    // posizione 2 all'interno della bitmap
#define POS3                 0x30    // posizione 3 all'interno della bitmap
#define POS4                 0x40    // posizione 4 all'interno della bitmap
#define POS5                 0x50    // posizione 5 all'interno della bitmap
#define POS6                 0x60    // posizione 6 all'interno della bitmap
#define POS7                 0x70    // posizione 7 all'interno della bitmap

/* bit 7 */
#define CF_ABS               0x00    // conversione C/F di un valore assoluto (F = (C * 1,8) + 32)
#define CF_REL               0x80    // conversione C/F di un valore relativo (F = C * 1,8)


/* Definizioni flags 2

/* bit 0-2 */
#define TYP_WORD             0x01    // WORD
#define TYP_BYTE             0x02    // BYTE
#define TYP_NIBB             0x03    // NIBBLE
#define TYP_HNIB             0x04    // HALF NIBBLE
#define TYP__BIT             0x05    // BIT

/* bit 3 */
#define RAM_MCU              0x00    // dato salvato nella ram del micro
#define RAM_RTC              0x08    // dato salvato nella non-volatile ram dell'RTC

/* bit 4 */
#define MAX_ABS              0x00    // valore massimo assoluto
#define MAX_REL              0x10    // valore massimo relativo

/* bit 5 */
#define MIN_ABS              0x00    // valore minimo assoluto
#define MIN_REL              0x20    // valore minimo relativo
