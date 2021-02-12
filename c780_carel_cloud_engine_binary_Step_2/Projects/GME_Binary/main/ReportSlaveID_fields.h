


/* refer to the document 
   svn > designstandard\Controller identification\Comand17 standard answer.xlsx
*/

#define CMD17_BASE   0


/* 
these defines point to the matching byte of the mnemonic name 
so to use it you need to perform a RepostSlaveID command and 
handle the result in a byte aray
*/
/*yellow section */
#define CMD17_MACHINE TYPE              (CMD17_BASE +  5)
#define CMD17_FW_RELEASE                (CMD17_BASE +  7)
#define CMD17_HW_CODE                   (CMD17_BASE +  9)
#define CMD17_PROTOCOLCAP               (CMD17_BASE + 11)
#define CMD17_REG_MAX_VAR               (CMD17_BASE + 13)
#define CMD17_COIL MAX_VAR              (CMD17_BASE + 14)
#define CMD17_OPTION_CODE               (CMD17_BASE + 16)
#define CMD17_EXPORT_CODE               (CMD17_BASE + 18)
#define CMD17_APPLICATION_RELEASE       (CMD17_BASE + 19)
#define CMD17_APP_CODE                  (CMD17_BASE + 20)
#define CMD17_CUSTOMER_CODE             (CMD17_BASE + 22)

/*light pink section */
#define CMD17_BACK_COMPATIBILITY        (CMD17_BASE + 24)
#define CMD17_PROTOCOL_REVISION         (CMD17_BASE + 25)
#define CMD17_UID_CAREL                 (CMD17_BASE + 27)

/* baby blue color */
#define CMD17_LENGHT                    (CMD17_BASE + 35 )
#define CMD17_TYPE                      (CMD17_BASE + 36 )
#define CMD17_VERSION                   (CMD17_BASE + 37 )
#define CMD17_PROJECT_GUID              (CMD17_BASE + 38 )
#define CMD17_PROJECT_RELEASE           (CMD17_BASE + 54 )
#define CMD17_OEM_ID                    (CMD17_BASE + 58 )
#define CMD17_MACHINE_CODE              (CMD17_BASE + 74 )
#define CMD17_OV_OEM_ID                 (CMD17_BASE + 90 )
#define CMD17_OV_MACHINE CODE           (CMD17_BASE + 106)
                                                     



#define UNLOCK_SPECIAL_FEATURES_PRESENT_MASK (1<<11)

/* field are ID*/													 
												 
#define TYPE_IS_APPLICA     1
#define TYPE_IS_ACU_BCU     2
#define TYPE_IS_CPCO        3
#define TYPE_IS_PCO         4
/*5 ? */
#define TYPE_IS_FW_VERSION  6


													 