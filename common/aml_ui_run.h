/*
 * Copyright (C) 2017 Amlogic Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSEERROR_CODE_INVALID_OPERATION.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  DESCRIPTION
 *      This file implements a adaptor of audio decoder from Amlogic.
 *
*/
#ifndef __AML_UI_RUN_H__
#define __AML_UI_RUN_H__
#ifdef __cplusplus
extern "C" {
#endif

int swupdateui_run(int argc, char *argv[]);

#define SWU_UI_DBG     0
#if     SWU_UI_DBG
#define DBG_MSG(...)   printf(__VA_ARGS__)
#else
#define DBG_MSG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif