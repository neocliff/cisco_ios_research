/* $Id: sr_atmaddregmib.c,v 3.7.8.1 1996/04/19 18:40:05 jjohnson Exp $
 * $Source: /release/112/cvs/Xsys/atm/sr_atmaddregmib.c,v $
 *------------------------------------------------------------------
 * ILMI Address Registration MIB
 *
 * March, 1995   Kartik Chandrasekhar
 *
 * Copyright (c) 1995-1997 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: sr_atmaddregmib.c,v $
 * Revision 3.7.8.1  1996/04/19  18:40:05  jjohnson
 * CSCdi55117:  Need to support the Draft Standard SNMPv2 SMI
 * Branch: California_branch
 * Prototypes for the <foo>_free functions are auto-generated by the
 * SR 14.0 MIB Compiler.  Make the declarations match the prototypes.
 *
 * Revision 3.7  1996/02/27  17:31:59  bstewart
 * CSCdi48325:  SNMP MIBs need modularized translation tables
 *
 * Revision 3.6  1996/02/01  05:59:55  hampton
 * Migrate files out of the parser directory.  [CSCdi47717]
 *   Step 2: Change all files which reference the moved parser files to
 *   point to their new locations.
 *
 * Revision 3.5  1996/01/24  22:07:28  anke
 * CSCdi47169:  Remove snmp from mibs as a required subsystem
 *
 * Revision 3.4  1996/01/18  15:43:12  anke
 * CSCdi46969:  Change empty req and/or seq strings in SUBSYS_HEADERs to
 *              NULL
 *              Five bytes saved is five bytes earned
 *
 * Revision 3.3  1995/12/14  08:22:41  jjohnson
 * CSCdi44148:  rationalize snmp library services
 *
 * Revision 3.2  1995/11/17  08:47:07  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  11:01:02  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.3  1995/06/28  09:22:20  smackie
 * Repair widespread subsystem header braindamage. (CSCdi23568)
 *
 *   o Fix subsystem entry points to be foo(subsystype *) not foo(void)
 *   o Repair nonsensical and redundant subsystem sequences
 *   o Use NULL where no property is required
 *
 * Revision 2.2  1995/06/18  20:31:51  dkatz
 * CSCdi36037:  Tons of memory wasted on no memory messages
 * Phase II--remove error messages.  Remove type coercion of malloc calls.
 *
 * Revision 2.1  1995/06/07  20:12:43  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 *
 * Copyright (C) 1994 by SNMP Research, Incorporated.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by SNMP Research, Incorporated.
 *
 * Restricted Rights Legend:
 *  Use, duplication, or disclosure by the Government is subject to
 *  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
 *  in Technical Data and Computer Software clause at DFARS 52.227-7013
 *  and in similar clauses in the FAR and NASA FAR Supplement.
 *
 */

#include "master.h"
#include "subsys.h"
#include "../snmp/snmp_api.h"
#include "sr_atmaddregmib.h"
#include "sr_atmaddregmib-mib.h"
#include "../snmp/snmp_registry.h"

#include "address.h"
#include "interface_private.h"
#include "../if/atm.h"
#include "parser_defs_atm.h"	/* bogus, but atm.h references it */
#include "ilmi_public.h"
#include "ilmi_private.h"

#define MAXBUFSIZE	32

void
init_atmaddregmib (subsystype *subsys)
{
    load_mib(atmaddregmib_OidList, atmaddregmib_OidListNum);
    load_oid(atmaddregmib_oid_table);
}




/*---------------------------------------------------------------------
 * Retrieve data from the atmfAddressEntry family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
atmfAddressEntry_get(
    OID            *incoming,
    ObjectInfo     *object,
    int             searchType,
    ContextInfo    *contextInfo,
    int             serialNum)
{
    int             instLength = incoming->length - object->oid.length;
    int             arg = object->nominator;
    void           *dp;
    atmfAddressEntry_t *data;
    unsigned long   buffer[MAXBUFSIZE];
    OID             inst;
    int             carry;
    int             i;
    long            atmfAddressPort;
    int             atmfAddressPort_offset;
    OctetString    *atmfAddressAtmAddress;
    int             atmfAddressAtmAddress_offset;
    int             index;
    int             final_index;
	addrtype		*intf;

    data = NULL;	/* make the compiler shut up */
    
	if(ilmi_type == ILMI_USERSIDE)
		return ((VarBind *) NULL);

    atmfAddressPort_offset = object->oid.length;
    atmfAddressAtmAddress_offset = atmfAddressPort_offset + 1;
    final_index = atmfAddressAtmAddress_offset + GetVariableIndexLength(incoming, atmfAddressAtmAddress_offset);

    if (searchType == EXACT) {
	if (instLength != (final_index - object->oid.length)) {
	    return ((VarBind *) NULL);
	}
	carry = 0;
    } else {
	carry = 1;
    }

    if ((InstToVariableOctetString(incoming, atmfAddressAtmAddress_offset, &atmfAddressAtmAddress, searchType, &carry)) < 0) {
	arg = -1;
    }
    if ((InstToInt(incoming, atmfAddressPort_offset, &atmfAddressPort, searchType, &carry)) < 0) {
	arg = -1;
    }
    if (carry) {
	arg = -1;
    }

	/*
	 * If the index value is zero then get the local interface number that
	 * is the one on which the SNMP pkt came in, and assign it to
	 * atmfAddressPort.
	 */
	 if(!atmfAddressPort) {
     	if(!(intf = reg_invoke_snmp_packet_source()))
			return ((VarBind *) NULL);
     	atmfAddressPort = intf->addr.ilmi_address;
	 }

    /*
     * Retrieve the data from the kernel-specific routine.
     */
    if ((arg == -1) || (data = k_atmfAddressEntry_get(serialNum, contextInfo, arg, searchType, atmfAddressPort, atmfAddressAtmAddress)) == NULL) {
	arg = -1;
    } else {
	/*
	 * Build instance information
	 */
	if (data->atmfAddressAtmAddress->length > MAXBUFSIZE)
	    return(NULL);	/* we'd overwrite the stack */
	inst.oid_ptr = buffer;
	index = 0;

    /*
     * In the ILMI case the port instance is always zero. Since the
     * prefix is defined as a variable length octet string the
     * length has to precede the address in the instance specification.
     */

    inst.oid_ptr[index++] = 0; /* Always zero */
    inst.oid_ptr[index++] = data->atmfAddressAtmAddress->length;

	for (i = 0; i < data->atmfAddressAtmAddress->length; i++) {
	    inst.oid_ptr[index++] = (unsigned long) data->atmfAddressAtmAddress->octet_ptr[i];
	}

	inst.length = index;
    }

    if (atmfAddressAtmAddress != NULL) {
	FreeOctetString(atmfAddressAtmAddress);
    }
    /*
     * Build the the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_atmfAddressStatus
      case I_atmfAddressStatus:
	dp = &data->atmfAddressStatus;
	break;
#endif				       /* I_atmfAddressStatus */

      default:
	return ((VarBind *) NULL);

    }				       /* switch */

    return (MakeVarBind(object, &inst, dp));

}

#ifdef SETS

/*----------------------------------------------------------------------
 * Free the atmfAddressEntry data object.
 *---------------------------------------------------------------------*/
void
atmfAddressEntry_free(atmfAddressEntry_t *data)
{
    if (data != NULL) {
	free((char *) data);
    }
}

/*----------------------------------------------------------------------
 * cleanup after atmfAddressEntry set/undo
 *---------------------------------------------------------------------*/
static int
atmfAddressEntry_cleanup(doList_t *trash)
{
    atmfAddressEntry_free(trash->data);
#ifdef SR_SNMPv2
    atmfAddressEntry_free(trash->undodata);
#endif				       /* SR_SNMPv2 */
    return NO_ERROR;
}

/*---------------------------------------------------------------------
 * Determine if this SET request is valid. If so, add it to the do-list.
 * This operation is performed in 4 steps:
 *
 *   1) Validate the object instance.
 *   2) Locate a "related" do-list element, or create a new do-list
 *      element.
 *   3) Add the SET request to the selected do-list element.
 *   4) Finally, determine if the do-list element (all "related" set
 *      requests) is completely valid.
 *---------------------------------------------------------------------*/
int
atmfAddressEntry_test(
    OID            *incoming,
    ObjectInfo     *object,
    ObjectSyntax   *value,
    doList_t       *doHead,
    doList_t       *doCur,
    ContextInfo    *contextInfo)
{
    int             instLength = incoming->length - object->oid.length;
    doList_t       *dp;
    int             found;
    int             carry = 0;
	/*
    atmfAddressEntry_t *atmfAddressEntry;
	*/
    long            atmfAddressPort;
    int             atmfAddressPort_offset;
    OctetString    *atmfAddressAtmAddress;
    int             atmfAddressAtmAddress_offset;
    int             final_index;
	ilmi_intfblock *intfblk=NULL ;
	addrtype		*intf;


	 /* 
	  * If the ilmi_type is the user-side then this i.e the Address
	  * Table is not suppported.
	  */
	if(ilmi_type == ILMI_USERSIDE)
		return (NO_ACCESS_ERROR);

     /* 
	  * The User-side is trying to set an address. It can do so only
	  * if the underlying interface is in the ilmiIntfUpAndNormal
	  * state. The interface is identified by the ilmi_snmp_intfnum
	  * which is a global variable indicating the interface on which 
	  * the request came in. It is set by the SNMP Engine. Further 
	  * the address has to be validated by the ILMI and by ILMI
	  * Clients who have registered to validate.
	  */
		

    /*
     * Validate the object instance
     */

    atmfAddressPort_offset = object->oid.length;
    atmfAddressAtmAddress_offset = atmfAddressPort_offset + 1;
    final_index = atmfAddressAtmAddress_offset + GetVariableIndexLength(incoming, atmfAddressAtmAddress_offset);

	if (instLength != (final_index - object->oid.length)) {
		return (NO_CREATION_ERROR);
    }

    if ((InstToInt(incoming, atmfAddressPort_offset, &atmfAddressPort, EXACT, &carry)) < 0) {
	return (NO_CREATION_ERROR);
    }
    if ((InstToVariableOctetString(incoming, atmfAddressAtmAddress_offset, &atmfAddressAtmAddress, EXACT, &carry)) < 0) {
	return (NO_CREATION_ERROR);
    }


	/*
	 * If the index value is zero then get the local interface number that
	 * is the one on which the SNMP pkt came in, and assign it to
	 * atmfAddressPort.
	 */

	if(!atmfAddressPort) {
    	if(!(intf = reg_invoke_snmp_packet_source()))
			return (NO_CREATION_ERROR);
     	atmfAddressPort = intf->addr.ilmi_address;
	}

	if( (intfblk = ilmi_getintfblk (atmfAddressPort)) == NULL)
		return (NO_CREATION_ERROR);  

	 /*
	  * Can we accept the address ?
	  */
	 if (intfblk->state != ilmiIntfUpAndNormal)
		return (NO_ACCESS_ERROR);

	/*
	 * Verify with Clients if they have a problem with this address request.
	 */

    if(ilmi_validate_address(atmfAddressPort,atmfAddressAtmAddress,value->sl_value) != ILMI_OK) 
		return (NO_CREATION_ERROR);  


    /*
     * Determine the element of the do-list which should contain this SET
     * request. This is decided by looking for a previous do-list element for
     * the same row.
     */
    found = 0;

    for (dp = doHead; dp != NULL; dp = dp->next) {
	if ((dp->setMethod == atmfAddressEntry_set) &&
	    (((atmfAddressEntry_t *) (dp->data)) != NULL) &&
	    (((atmfAddressEntry_t *) (dp->data))->atmfAddressPort == atmfAddressPort) &&
	    (((atmfAddressEntry_t *) (dp->data))->atmfAddressAtmAddress == atmfAddressAtmAddress)) {

	    found = 1;
	    break;
	}
    }

    if (!found) {
	dp = doCur;
	if ((dp->data = malloc(sizeof(atmfAddressEntry_t))) == NULL) {
	    DPRINTF((APALWAYS, "snmpd: Cannot allocate memory\n"));
	    return (RESOURCE_UNAVAILABLE_ERROR);
	}
	memset(dp->data, 0, sizeof(atmfAddressEntry_t));

	dp->setMethod = atmfAddressEntry_set;
	dp->cleanupMethod = atmfAddressEntry_cleanup;
#ifdef SR_SNMPv2
	dp->undoMethod = NULL;
#endif				       /* SR_SNMPv2 */
	dp->state = UNKNOWN;

	SET_VALID(I_atmfAddressPort, ((atmfAddressEntry_t *) (dp->data))->valid);
	((atmfAddressEntry_t *) (dp->data))->atmfAddressPort = atmfAddressPort;

	SET_VALID(I_atmfAddressAtmAddress, ((atmfAddressEntry_t *) (dp->data))->valid);
	((atmfAddressEntry_t *) (dp->data))->atmfAddressAtmAddress = atmfAddressAtmAddress;

    }
    switch (object->nominator) {

#ifdef I_atmfAddressStatus
      case I_atmfAddressStatus:

	SET_VALID(I_atmfAddressStatus, ((atmfAddressEntry_t *) (dp->data))->valid);

	((atmfAddressEntry_t *) (dp->data))->atmfAddressStatus = value->sl_value;
	break;
#endif				       /* I_atmfAddressStatus */

      default:
	DPRINTF((APALWAYS, "snmpd: Internal error (invalid nominator in atmfAddressEntry_test)\n"));
	return (GEN_ERROR);

    }				       /* switch */
    dp->state = ADD_MODIFY;

    return (NO_ERROR);

}

/*---------------------------------------------------------------------
 * Perform the kernel-specific set function for this group of
 * related objects.
 *---------------------------------------------------------------------*/
int
atmfAddressEntry_set(doHead, doCur, contextInfo)
    doList_t       *doHead;
    doList_t       *doCur;
    ContextInfo    *contextInfo;
{
    return (k_atmfAddressEntry_set((atmfAddressEntry_t *) (doCur->data),
				   contextInfo, doCur->state));
}

#endif				       /* SETS */


/*---------------------------------------------------------------------
 * Retrieve data from the atmfNetPrefixEntry family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
atmfNetPrefixEntry_get(incoming, object, searchType, contextInfo, serialNum)
    OID            *incoming;
    ObjectInfo     *object;
    int             searchType;
    ContextInfo    *contextInfo;
    int             serialNum;
{
    int             instLength = incoming->length - object->oid.length;
    int             arg = object->nominator;
    void           *dp;
    atmfNetPrefixEntry_t *data;
    unsigned long   buffer[MAXBUFSIZE];
    OID             inst;
    int             carry;
    int             i;
    long            atmfNetPrefixPort;
    int             atmfNetPrefixPort_offset;
    OctetString    *atmfNetPrefixPrefix;
    int             atmfNetPrefixPrefix_offset;
    int             index;
    int             final_index;
	addrtype		*intf;

    data = NULL;	/* make compiler shut up */

	if(ilmi_type == ILMI_NETWORKSIDE)
		return ((VarBind *) NULL);
    
    atmfNetPrefixPort_offset = object->oid.length;
    atmfNetPrefixPrefix_offset = atmfNetPrefixPort_offset + 1;
    final_index = atmfNetPrefixPrefix_offset + GetVariableIndexLength(incoming, atmfNetPrefixPrefix_offset);

    if (searchType == EXACT) {
	if (instLength != (final_index - object->oid.length)) {
	    return ((VarBind *) NULL);
	}
	carry = 0;
    } else {
	carry = 1;
    }

    if ((InstToVariableOctetString(incoming, atmfNetPrefixPrefix_offset, &atmfNetPrefixPrefix, searchType, &carry)) < 0) {
	arg = -1;
    }
    if ((InstToInt(incoming, atmfNetPrefixPort_offset, &atmfNetPrefixPort, searchType, &carry)) < 0) {
	arg = -1;
    }
    if (carry) {
	arg = -1;
    }

    /*
     * If the index value is zero then get the local interface number that
     * is the one on which the SNMP pkt came in, and assign it to
     * atmfNetPrefixPort.
     */

    if(!atmfNetPrefixPort) {
     	if(!(intf = reg_invoke_snmp_packet_source())) {
	    if (atmfNetPrefixPrefix != NULL) {
		FreeOctetString(atmfNetPrefixPrefix);
	    }
	    return ((VarBind *) NULL);
	}
     	atmfNetPrefixPort = intf->addr.ilmi_address;
    }

    /*
     * Retrieve the data from the kernel-specific routine.
     */
    if ((arg == -1) || (data = k_atmfNetPrefixEntry_get(serialNum, contextInfo, arg, searchType, atmfNetPrefixPort, atmfNetPrefixPrefix)) == NULL) {
	arg = -1;
    } else {
	/*
	 * Build instance information
	 */
	if (data->atmfNetPrefixPrefix->length > MAXBUFSIZE)
	    return(NULL);	/* we'd overwrite the stack */
	inst.oid_ptr = buffer;
	index = 0;

    /*
     * In the ILMI case the port instance is always zero. Since the
     * prefix is defined as a variable length octet string the
     * length has to precede the prefix in the instance specification.
     */

    inst.oid_ptr[index++] = 0; /* Always zero */
    inst.oid_ptr[index++] = data->atmfNetPrefixPrefix->length;

	for (i = 0; i < data->atmfNetPrefixPrefix->length; i++) {
	    inst.oid_ptr[index++] = (unsigned long) data->atmfNetPrefixPrefix->octet_ptr[i];
	}

	inst.length = index;
    }

    if (atmfNetPrefixPrefix != NULL) {
	FreeOctetString(atmfNetPrefixPrefix);
    }
    
    /*
     * Build the the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_atmfNetPrefixStatus
      case I_atmfNetPrefixStatus:
	dp = &data->atmfNetPrefixStatus;
	break;
#endif				       /* I_atmfNetPrefixStatus */

      default:
	return ((VarBind *) NULL);

    }				       /* switch */

    return (MakeVarBind(object, &inst, dp));

}

#ifdef SETS

/*----------------------------------------------------------------------
 * Free the atmfNetPrefixEntry data object.
 *---------------------------------------------------------------------*/
void
atmfNetPrefixEntry_free (atmfNetPrefixEntry_t *data)
{
    if (data != NULL) {
	if (data->atmfNetPrefixPrefix)
	    FreeOctetString(data->atmfNetPrefixPrefix);
	free((char *) data);
    }
}

/*----------------------------------------------------------------------
 * cleanup after atmfNetPrefixEntry set/undo
 *---------------------------------------------------------------------*/
static int
atmfNetPrefixEntry_cleanup (doList_t *trash)
{
    atmfNetPrefixEntry_free(trash->data);
#ifdef SR_SNMPv2
    atmfNetPrefixEntry_free(trash->undodata);
#endif				       /* SR_SNMPv2 */
    return NO_ERROR;
}

/*---------------------------------------------------------------------
 * Determine if this SET request is valid. If so, add it to the do-list.
 * This operation is performed in 4 steps:
 *
 *   1) Validate the object instance.
 *   2) Locate a "related" do-list element, or create a new do-list
 *      element.
 *   3) Add the SET request to the selected do-list element.
 *   4) Finally, determine if the do-list element (all "related" set
 *      requests) is completely valid.
 *---------------------------------------------------------------------*/
int
atmfNetPrefixEntry_test(incoming, object, value, doHead, doCur, contextInfo)
    OID            *incoming;
    ObjectInfo     *object;
    ObjectSyntax   *value;
    doList_t       *doHead;
    doList_t       *doCur;
    ContextInfo    *contextInfo;
{
    int             instLength = incoming->length - object->oid.length;
    doList_t       *dp;
    int             found;
    int             carry = 0;
    long            atmfNetPrefixPort;
    int             atmfNetPrefixPort_offset;
    OctetString    *atmfNetPrefixPrefix = NULL;
    int             atmfNetPrefixPrefix_offset;
    int             final_index;
	ilmi_intfblock *intfblk=NULL ;
	addrtype		*intf;

	 /* 
	  * If the ilmi_type is the network-side then this i.e the Prefix
	  * Table is not suppported.
	  */
	if(ilmi_type == ILMI_NETWORKSIDE)
		return (NO_ACCESS_ERROR);

     /* 
	  * The Network-side is trying to set an address (Prefix). It can do so 
	  * only if the underlying interface is in the ilmiIntfUpAndNormal
	  * state. The interface is identified by the ilmi_snmp_intfnum which is 
	  * a global variable indicating the interface on which the request came 
	  * in. It is set by the SNMP Engine. Further the address has to be 
	  * validated by the ILMI and by ILMI  Clients who have registered to 
	  * validate.
	  */
	  

    /*
     * Validate the object instance
     * 
     */
    atmfNetPrefixPort_offset = object->oid.length;
    atmfNetPrefixPrefix_offset = atmfNetPrefixPort_offset + 1;
    final_index = atmfNetPrefixPrefix_offset + GetVariableIndexLength(incoming, atmfNetPrefixPrefix_offset);

	if (instLength != (final_index - object->oid.length)) {
		return (NO_CREATION_ERROR);
    }

    if ((InstToInt(incoming, atmfNetPrefixPort_offset, &atmfNetPrefixPort, EXACT, &carry)) < 0) {
	return (NO_CREATION_ERROR);
    }
    if ((InstToVariableOctetString(incoming, atmfNetPrefixPrefix_offset, &atmfNetPrefixPrefix, EXACT, &carry)) < 0) {
	return (NO_CREATION_ERROR);
    }

    /*
     * If the index value is zero then get the local interface number that
     * is the one on which the SNMP pkt came in, and assign it to
     * atmfNetPrefixPort.
     */

    if(!atmfNetPrefixPort) {
	if(!(intf = reg_invoke_snmp_packet_source())) {
	    if (atmfNetPrefixPrefix != NULL) {
		FreeOctetString(atmfNetPrefixPrefix);
	    }
	    return (NO_CREATION_ERROR);
	}
	atmfNetPrefixPort = intf->addr.ilmi_address;
    }


    if( (intfblk = ilmi_getintfblk (atmfNetPrefixPort)) == NULL) {
	if (atmfNetPrefixPrefix != NULL) {
	    FreeOctetString(atmfNetPrefixPrefix);
	}
	return (NO_CREATION_ERROR);
    }

    if((!intfblk->idb) || (!intfblk->idb->atm_db)) {
	if (atmfNetPrefixPrefix)
	    FreeOctetString(atmfNetPrefixPrefix);
	return (NO_CREATION_ERROR);
    }

    if (!intfblk->idb->atm_db->address_reg) {
	if (atmfNetPrefixPrefix)
	    FreeOctetString(atmfNetPrefixPrefix);
	if (atm_ilmi_debug)
	    buginf("\nILMI(%s):Prefix rejected. Address registration disabled",
		   intfblk->idb->firstsw->namestring);
	return (NO_CREATION_ERROR);
    }

    if(intfblk->state != ilmiIntfUpAndNormal) {
	if (atmfNetPrefixPrefix != NULL) {
	    FreeOctetString(atmfNetPrefixPrefix);
	}
	return (NO_ACCESS_ERROR);
    }

    if(ilmi_validate_address(atmfNetPrefixPort,atmfNetPrefixPrefix,value->sl_value) != ILMI_OK) {
	if (atmfNetPrefixPrefix != NULL) {
	    FreeOctetString(atmfNetPrefixPrefix);
	}
	return (NO_CREATION_ERROR);
    }


    /*
     * Determine the element of the do-list which should contain this SET
     * request. This is decided by looking for a previous do-list element for
     * the same row.
     */
    found = 0;

    for (dp = doHead; dp != NULL; dp = dp->next) {
	if ((dp->setMethod == atmfNetPrefixEntry_set) &&
	    (((atmfNetPrefixEntry_t *) (dp->data)) != NULL) &&
	    (((atmfNetPrefixEntry_t *) (dp->data))->atmfNetPrefixPort == atmfNetPrefixPort) &&
	    (((atmfNetPrefixEntry_t *) (dp->data))->atmfNetPrefixPrefix == atmfNetPrefixPrefix)) {

	    found = 1;
	    break;
	}
    }

    if (!found) {
	dp = doCur;
	if ((dp->data = malloc(sizeof(atmfNetPrefixEntry_t))) == NULL) {
	    DPRINTF((APALWAYS, "snmpd: Cannot allocate memory\n"));
	    return (RESOURCE_UNAVAILABLE_ERROR);
	}
	memset(dp->data, 0, sizeof(atmfNetPrefixEntry_t));

	dp->setMethod = atmfNetPrefixEntry_set;
	dp->cleanupMethod = atmfNetPrefixEntry_cleanup;
#ifdef SR_SNMPv2
	dp->undoMethod = NULL;
#endif				       /* SR_SNMPv2 */
	dp->state = UNKNOWN;

	SET_VALID(I_atmfNetPrefixPort,((atmfNetPrefixEntry_t *)(dp->data))->valid);
	((atmfNetPrefixEntry_t *)(dp->data))->atmfNetPrefixPort = atmfNetPrefixPort;
	SET_VALID(I_atmfNetPrefixPrefix,((atmfNetPrefixEntry_t *)(dp->data))->valid);
	((atmfNetPrefixEntry_t *) (dp->data))->atmfNetPrefixPrefix = atmfNetPrefixPrefix;

    }
    switch (object->nominator) {

#ifdef I_atmfNetPrefixStatus
      case I_atmfNetPrefixStatus:

	SET_VALID(I_atmfNetPrefixStatus, ((atmfNetPrefixEntry_t *) (dp->data))->valid);

	((atmfNetPrefixEntry_t *) (dp->data))->atmfNetPrefixStatus = value->sl_value;
	break;
#endif				       /* I_atmfNetPrefixStatus */

      default:
	DPRINTF((APALWAYS, "snmpd: Internal error (invalid nominator in atmfNetPrefixEntry_test)\n"));
	return (GEN_ERROR);

    }				       /* switch */
    dp->state = ADD_MODIFY;

    return (NO_ERROR);

}

/*---------------------------------------------------------------------
 * Perform the kernel-specific set function for this group of
 * related objects.
 *---------------------------------------------------------------------*/
int
atmfNetPrefixEntry_set(doHead, doCur, contextInfo)
    doList_t       *doHead;
    doList_t       *doCur;
    ContextInfo    *contextInfo;
{
    return (k_atmfNetPrefixEntry_set((atmfNetPrefixEntry_t *) (doCur->data),
				     contextInfo, doCur->state));
}

#endif				       /* SETS */






/*
 * snmp_get_atmaddReg
 * Return the atm structure for the passed index, per the searchtype
 */
ilmi_atmaddressRow *
snmp_get_atmaddReg (int searchtype, long intf, OctetString *addr)
{
    return(ilmi_search_addrTab(intf,addr, searchtype));
}


atmfAddressEntry_t *
k_atmfAddressEntry_get(
    int             serialNum,
    ContextInfo    *contextInfo,
    int             nominator,
    int             searchType,
    long            atmfAddressPort,
    OctetString    *atmfAddressAtmAddress)
{
    static atmfAddressEntry_t atmfAddressEntryData;
    ilmi_atmaddressRow	   *atm;

	atm = NULL;
    atm = snmp_get_atmaddReg(searchType,atmfAddressPort,atmfAddressAtmAddress);
    if (atm == NULL)
		return(NULL);

    atmfAddressEntryData.atmfAddressPort = atm->intf_id;
    atmfAddressEntryData.atmfAddressAtmAddress = atm->addr;
    atmfAddressEntryData.atmfAddressStatus =  ((atm->status == ilmiAddressValid) ? D_atmfAddressStatus_valid  : D_atmfAddressStatus_invalid); 

    return (&atmfAddressEntryData);
}

#ifdef SETS
int
k_atmfAddressEntry_set(data, contextInfo, function)
    atmfAddressEntry_t *data;
    ContextInfo    *contextInfo;
    int             function;
{

	return snmp_set_atmaddReg(data->atmfAddressPort,data->atmfAddressAtmAddress,(data->atmfAddressStatus == D_atmfAddressStatus_valid) ? ilmiAddressValid : ilmiAddressInValid);

}

#endif				       /* SETS */

atmfNetPrefixEntry_t *
k_atmfNetPrefixEntry_get(
    int             serialNum,
    ContextInfo    *contextInfo,
    int             nominator,
    int             searchType,
    long            atmfNetPrefixPort,
    OctetString    *atmfNetPrefixPrefix)
{
    static atmfNetPrefixEntry_t atmfNetPrefixEntryData;
    ilmi_atmaddressRow	   *atm;

	atm = NULL;
    atm = snmp_get_atmaddReg(searchType,atmfNetPrefixPort,atmfNetPrefixPrefix);

    if (atm == NULL)
	return(NULL);

    atmfNetPrefixEntryData.atmfNetPrefixPort = atm->intf_id;
    atmfNetPrefixEntryData.atmfNetPrefixPrefix = atm->addr;
    atmfNetPrefixEntryData.atmfNetPrefixStatus =  ((atm->status == ilmiAddressValid) ? D_atmfNetPrefixStatus_valid  : D_atmfNetPrefixStatus_invalid); 

    return (&atmfNetPrefixEntryData);
}

#ifdef SETS
int
k_atmfNetPrefixEntry_set (
    atmfNetPrefixEntry_t *data,
    ContextInfo    *contextInfo,
    int             function)
{

	return ( snmp_set_atmaddReg(data->atmfNetPrefixPort,data->atmfNetPrefixPrefix,(data->atmfNetPrefixStatus == D_atmfNetPrefixStatus_valid ? ilmiAddressValid : ilmiAddressInValid)) );

}

#endif				       /* SETS */


/*
 * atmaddregmib subsystem header
 */


#define MAJVERSION_atmaddregmib 1
#define MINVERSION_atmaddregmib 0
#define EDITVERSION_atmaddregmib 0

SUBSYS_HEADER(atmaddregmib,
              MAJVERSION_atmaddregmib,
              MINVERSION_atmaddregmib,
              EDITVERSION_atmaddregmib,
              init_atmaddregmib,
              SUBSYS_CLASS_MANAGEMENT,
              NULL,
              NULL);
      