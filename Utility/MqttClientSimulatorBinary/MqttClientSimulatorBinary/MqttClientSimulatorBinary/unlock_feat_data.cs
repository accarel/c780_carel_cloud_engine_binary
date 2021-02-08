using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using PeterO;
using PeterO.Cbor;
using PeterO.Numbers;


namespace MqttClientSimulatorBinary
{
    class unlock_feat_data
    {

        /* feature TODAY 20210108 defined */
        public const int RW_STATUS_LOCKED      = 0x0000;
        public const int RW_STATUS_UNLOCKED    = 0x0001;
        public const int RW_STATUS_KEY_WRITTEN = 0x8000;
        public const int RW_STATUS_GET_SERVICE = 0x4000;
        
        /* feature TODAY 20210108 defined */
        public const int UNLOCK_BIT_STC   = (1);
        public const int UNLOCK_BIT_LOGS  = (1<<1);
        public const int UNLOCK_BIT_ALR   = (1<<2);
        public const int UNLOCK_BIT_UPG   = (1<<3);
        public const int UNLOCK_BIT_BLKRC = (1 << 4);

        //all the feature unlocked
        //public const int UNLOCK_BITS = (UNLOCK_BIT_STC | UNLOCK_BIT_LOGS | UNLOCK_BIT_ALR | UNLOCK_BIT_UPG | UNLOCK_BIT_BLKRC);
        public const UInt32 UNLOCK_BITS = (UInt32)(0xFFFFFFFF); 

        /* Modubus ADDRESS start here */
        public const int MB_ADDR_BASE = 60000;
        
        /* 4 byte */
        public const int MB_ADDR_SRV_VER_MAJ    =     (MB_ADDR_BASE + 0)    ;
        public const int MB_ADDR_SRV_VER_MIN    =     (MB_ADDR_BASE + 1)    ;
        /* 18 byte */                                          
        public const int MB_ADDR_PN_0           =     (MB_ADDR_BASE + 2)    ;
        public const int MB_ADDR_PN_1           =     (MB_ADDR_BASE + 3)    ;
        public const int MB_ADDR_PN_2           =     (MB_ADDR_BASE + 4)    ;
        public const int MB_ADDR_PN_3           =     (MB_ADDR_BASE + 5)    ;
        public const int MB_ADDR_PN_4           =     (MB_ADDR_BASE + 6)    ;
        public const int MB_ADDR_PN_5           =     (MB_ADDR_BASE + 7)    ;
        public const int MB_ADDR_PN_6           =     (MB_ADDR_BASE + 8)    ;
        public const int MB_ADDR_PN_7           =     (MB_ADDR_BASE + 9)    ;
        public const int MB_ADDR_PN_8           =     (MB_ADDR_BASE + 10)   ;
        /* 4 byte */
        public const int MB_ADDR_RND_NUM_H      =     (MB_ADDR_BASE + 11)   ; 
        public const int MB_ADDR_RND_NUM_L      =     (MB_ADDR_BASE + 12)   ; 
        /* 4 byte */                 
        public const int MB_ADDR_UNLOCK_BITS_H  =     (MB_ADDR_BASE + 13)   ;
        public const int MB_ADDR_UNLOCK_BITS_L  =     (MB_ADDR_BASE + 14)   ;
        /* 2 byte */               
        public const int MB_ADDR_RW_STATUS      =     (MB_ADDR_BASE + 15)   ;
        /* 4 byte */                                                
        public const int MB_ADDR_SESSION_UNLOCK_KEY_H = (MB_ADDR_BASE + 16)   ;
        public const int MB_ADDR_SESSION_UNLOCK_KEY_L = (MB_ADDR_BASE + 17)   ;

        /*
        starting from (MB_ADDR_BASE + 18) there are the FW/SW version section not used byte
        the gateway and for this reason left undefined
        */


        /* --------------------------------------------------------------------------- */
        public const int PARTNUMBER_SIZE = (18 / 2); //17 bytes but through register need to be rounded  BILATO TBC
        public const int OEM_ID_SIZE = 16;

        




    }
}






