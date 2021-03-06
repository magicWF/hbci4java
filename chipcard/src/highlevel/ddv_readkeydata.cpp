/**********************************************************************
 *
 * This file is part of HBCI4Java.
 * Copyright (c) 2001-2007 Stefan Palme
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 **********************************************************************/

#include <stdlib.h>

#include "ctapi-tools.h"
#include "hbci.h"
#include "seccos.h"

bool DDV_readKeyData(HBCI_KeyInfo **data,size_t *dataLen)
{
    bool ret=false;
    *dataLen=0;
    
    if (HBCI_cardtype==HBCI_CARD_TYPE_DDV_0) {
        CTAPI_log("reading ddv-0 keys");
        
        if (SECCOS_selectSubFile(SECCOS_SELECT_RET_NOTHING,0x0013)) {
            unsigned char *buffer=new unsigned char[16];
            size_t        len;
            
            if (SECCOS_readRecord(1,buffer,&len)) {
                HBCI_KeyInfo *entry=new HBCI_KeyInfo;
                
                entry->keynum=buffer[0x00];
                entry->keyversion=buffer[0x04];
                entry->keylen=buffer[0x01];
                entry->alg=buffer[0x02];
                
                data[0]=entry;
                (*dataLen)++;
                ret=true;
            }
            
            delete buffer;
        }

        if (SECCOS_selectSubFile(SECCOS_SELECT_RET_NOTHING,0x0014)) {
            unsigned char *buffer=new unsigned char[16];
            size_t        len;
            
            if (SECCOS_readRecord(1,buffer,&len)) {
                HBCI_KeyInfo *entry=new HBCI_KeyInfo;
                
                entry->keynum=buffer[0x00];
                entry->keyversion=buffer[0x03];
                entry->keylen=buffer[0x01];
                entry->alg=buffer[0x02];
                
                data[1]=entry;
                (*dataLen)++;
                ret&=true;
            }
            
            delete buffer;
        }
    } else if (HBCI_cardtype==HBCI_CARD_TYPE_DDV_1) {
        CTAPI_log("reading ddv-1 keys");
        
        unsigned char *buffer=new unsigned char[300];
        size_t        len;
        
        if (SECCOS_getKeyInfo(2,SECCOS_KEY_TYPE_DF,buffer,&len)) {
            HBCI_KeyInfo *entry=new HBCI_KeyInfo;
            
            entry->keynum=2;
            entry->keyversion=buffer[len-1];
            entry->keylen=0;
            entry->alg=0;
            
            data[0]=entry;
            (*dataLen)++;
            ret=true;
        }

        if (SECCOS_getKeyInfo(3,SECCOS_KEY_TYPE_DF,buffer,&len)) {
            HBCI_KeyInfo *entry=new HBCI_KeyInfo;
            
            entry->keynum=3;
            entry->keyversion=buffer[len-1];
            entry->keylen=0;
            entry->alg=0;
            
            data[1]=entry;
            (*dataLen)++;
            ret&=true;
        }
        
        delete buffer;
    } else if (HBCI_cardtype==HBCI_CARD_TYPE_RSA) {
        CTAPI_log("reading rsa keys");
    }
    
    return ret;
}
