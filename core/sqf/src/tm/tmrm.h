// @@@ START COPYRIGHT @@@
//
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// @@@ END COPYRIGHT @@@

#ifndef TMRM_H_
#define TMRM_H_

#include "tmmutex.h"

class CTmTxBase;
class CTmTxMessage;

// Pure virtual base class for Branches
class RM_Info
{
    protected:
      int      iv_num_rm_partic;
      TM_Mutex iv_branch_mutex;

    public:
      RM_Info(){iv_num_rm_partic=0;}
      ~RM_Info(){}
      void branch_lock();
      void branch_unlock();
      virtual void inc_num_rm_partic() {RM_Info::iv_num_rm_partic++;}

      virtual int32 num_rm_partic(CTmTxBase *pp_txn = 0) = 0;
      virtual int32 num_rms_unresolved(CTmTxBase *pp_txn = 0) = 0;
      virtual void reset_resolved(CTmTxBase *pp_txn = 0) = 0;
      virtual int32 num_rm_failed(CTmTxBase *pp_txn = 0) = 0;
      virtual void init_rms(CTmTxBase *pp_txn, bool pv_partic) = 0;

      virtual int32 shutdown_branches(bool pv_leadTM, bool pv_clean) = 0;
      
      // Generic branch interface
      virtual int32 commit_branches (CTmTxBase *pp_txn, int64 pv_flags, CTmTxMessage * pp_msg) = 0;    
      virtual int32 end_branches (CTmTxBase *pp_txn, int64 pv_flags) = 0;  
      virtual int32 forget_branches (CTmTxBase *pp_txn, int64 pv_flags) = 0;    
      virtual int32 forget_heur_branches (CTmTxBase *pp_txn, int64 pv_flags) = 0; 
      virtual int32 prepare_branches (CTmTxBase *pp_txn, int64 pv_flags) = 0;
      virtual int32 rollback_branches (CTmTxBase *pp_txn, int64 pv_flags, CTmTxMessage * pp_msg,
                              bool lv_error_condition = false) = 0;       
      virtual int32 start_branches (CTmTxBase *pp_txn, int64 pv_flags, CTmTxMessage * pp_msg) = 0;
      virtual int32 registerRegion (CTmTxBase *pp_txn, int64 pv_flags, CTmTxMessage * pp_msg) = 0;
};
 
#endif // TMRM_H_