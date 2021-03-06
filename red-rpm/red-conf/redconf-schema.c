/*
* Copyright (C) 2020 "IoT.bzh"
* Author Fulup Ar Foll <fulup@iot.bzh>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Documentation: https://github.com/tlsa/libcyaml/blob/master/docs/guide.md
*/
#define _GNU_SOURCE
#include <rpm/rpmlog.h>
#include "redconf.h"


static const cyaml_config_t yconfError= {
    .log_level = CYAML_LOG_ERROR, 
    .log_fn = cyaml_log, 
    .mem_fn = cyaml_mem, 
};

static const cyaml_config_t yconfWarning = {
    .log_level = CYAML_LOG_WARNING, 
    .log_fn = cyaml_log, 
    .mem_fn = cyaml_mem, 
};    

static const cyaml_config_t yconfNotice = {
    .log_level = CYAML_LOG_NOTICE, 
    .log_fn = cyaml_log, 
    .mem_fn = cyaml_mem, 
};

static const cyaml_config_t yconfInfo = {
    .log_level = CYAML_LOG_INFO, 
    .log_fn = cyaml_log,  
    .mem_fn = cyaml_mem, 
};

static const cyaml_config_t yconfDebug= {
    .log_level = CYAML_LOG_DEBUG, 
    .log_fn = cyaml_log, 
    .mem_fn = cyaml_mem, 
};

static const cyaml_config_t *yconft[] ={
   &yconfError,
   &yconfWarning,
   &yconfInfo,
   &yconfNotice,
   &yconfDebug,
};

static const cyaml_config_t *yconfGet (int wlevel) {
    static int maxLevel = (int) (sizeof(yconft) / sizeof(cyaml_config_t*)) -1;

    if (wlevel > maxLevel) {
        rpmlog(REDLOG_ERROR, "Fail yconf verbosity wlevel too high val=%d max=%d", wlevel, maxLevel);
        return NULL;
    }
    return yconft[wlevel];
}

// --- Red Status Scheùa parse ${redpath}/.status ----

    const cyaml_strval_t statusFlagStrings[] = {
        { "Disable", RED_STATUS_DISABLE},
        { "Enable",  RED_STATUS_ENABLE},
        { "Unknown", RED_STATUS_UNKNOWN},
        { "Error",   RED_STATUS_ERROR},
    };

    // status data
    static const cyaml_schema_field_t StatusEntry[] = {    /// trouve le bon type pour count check les tasks
        CYAML_FIELD_STRING_PTR("realpath", CYAML_FLAG_POINTER, redStatusT, realpath, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("info", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redStatusT, info, 0, CYAML_UNLIMITED),
        CYAML_FIELD_UINT("timestamp", CYAML_FLAG_POINTER, redStatusT, timestamp),
        CYAML_FIELD_ENUM("state", CYAML_FLAG_DEFAULT, redStatusT, state, statusFlagStrings, CYAML_ARRAY_LEN(statusFlagStrings)),
        CYAML_FIELD_END
    };

    // Top wlevel schema entry point must be a unique CYAML_VALUE_MAPPING
    static const cyaml_schema_value_t StatusTopSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, redConfigT, StatusEntry),
    };

    // ---- Red config Schema parse ${redpath}/etc/redpack.yaml ----
    const cyaml_strval_t exportFlagStrings[] = {
        { "Restricted", RED_EXPORT_RESTRICTED},
        { "Public",     RED_EXPORT_PUBLIC},
        { "Private",    RED_EXPORT_PRIVATE},
    };

    // redpak config headers schema
    static const cyaml_schema_field_t HeaderSchema[] = {
        CYAML_FIELD_STRING_PTR("alias", CYAML_FLAG_POINTER, redConfHeaderT, alias, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfHeaderT, name, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("info", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfHeaderT, info, 0,CYAML_UNLIMITED),
        CYAML_FIELD_END
    };

    // mounting point label+path
    static const cyaml_schema_field_t ExportEntry[] = {    
        CYAML_FIELD_STRING_PTR("mount", CYAML_FLAG_POINTER, redConfExportPathT, mount, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("path", CYAML_FLAG_POINTER, redConfExportPathT, path, 0, CYAML_UNLIMITED),
        CYAML_FIELD_ENUM("mode", CYAML_FLAG_STRICT|CYAML_FLAG_OPTIONAL, redConfExportPathT, mode, exportFlagStrings, CYAML_ARRAY_LEN(exportFlagStrings)),
        CYAML_FIELD_END
    };

    // relocation old/new path
    static const cyaml_schema_field_t RelocationEntry[] = {    
        CYAML_FIELD_STRING_PTR("old", CYAML_FLAG_POINTER, redConfRelocationT, old, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("new", CYAML_FLAG_POINTER, redConfRelocationT, new, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
    };

    // dnf/rpm packages options
    static const cyaml_schema_field_t PkgSchema[] = {   
        CYAML_FIELD_STRING_PTR("persistdir", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, persistdir, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("rpmdir", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, rpmdir, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("path", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, path, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR("ldpath", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, ldpath, 0, CYAML_UNLIMITED),
        CYAML_FIELD_INT("verbose", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, verbose),
        CYAML_FIELD_INT("maxage", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, maxage),
        CYAML_FIELD_INT("umask", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, umask),
        CYAML_FIELD_BOOL("gpgcheck", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, gpgcheck),
        CYAML_FIELD_BOOL("inherit", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, inherit),
        CYAML_FIELD_BOOL("forcemod", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfVarT, forcemod),
        CYAML_FIELD_END
    };

    static const cyaml_schema_value_t ExportSchema = {CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,redConfExportPathT, ExportEntry),};
    static const cyaml_schema_value_t RelocsSchema = {CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,redConfRelocationT, RelocationEntry),};

    // First wlevel config structure (id, export, acl, status)
    static const cyaml_schema_field_t RedConfigSchema[] = {
        CYAML_FIELD_MAPPING_PTR("headers", CYAML_FLAG_POINTER, redConfigT , headers, HeaderSchema),
        CYAML_FIELD_SEQUENCE("exports", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfigT , exports, &ExportSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE("relocations", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfigT , exports, &RelocsSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR("environment", CYAML_FLAG_POINTER|CYAML_FLAG_OPTIONAL, redConfigT , environment, PkgSchema),
        CYAML_FIELD_END
    };

    // Top wlevel schema entry point must be a unique CYAML_VALUE_MAPPING
    static const cyaml_schema_value_t ConfTopSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, redConfigT, RedConfigSchema),
    };
// ---- end ${redpath}/etc/redpack.yaml schema ----


static int SchemaSave (const char* filepath, const cyaml_schema_value_t *topschema, void *config, int wlevel) {
    int errcode=0;

    const cyaml_config_t *yconf = yconfGet (wlevel);
    if (!yconf) return (-1);

    errcode = cyaml_save_file(filepath, yconf, topschema, config, 0);
	if (errcode != CYAML_OK) {
        rpmlog(REDLOG_ERROR, "Fail to reading yaml config path=%s err=[%s]", filepath, cyaml_strerror(errcode));
    }
 
    return errcode;
}

static int SchemaLoad (const char* filepath, const cyaml_schema_value_t *topschema, void **config, int wlevel) {
    int errcode=0;

    // select parsing log wlevel
    const cyaml_config_t *yconf = yconfGet (wlevel);
    if (!yconf) return (-1);

    errcode = cyaml_load_file(filepath, yconf, topschema, config, 0);

	if (errcode != CYAML_OK) {
        // when error reparse with higger level of debug to make visible errors
        if (wlevel == 1) 
            errcode = SchemaLoad (filepath, topschema, config, wlevel+1);
        else     
            rpmlog(REDLOG_ERROR, "Fail to reading yaml config path=%s err=[%s]", filepath, cyaml_strerror(errcode));
    }
 
    return errcode;
}

int RedSaveConfig (const char* filepath, redConfigT *config, int warning ) {
    int errcode = SchemaSave(filepath, &ConfTopSchema, (void*)config, 0);
    return errcode;
}

int RedSaveStatus (const char* filepath, redStatusT *status, int warning ) {
    int errcode = SchemaSave(filepath, &StatusTopSchema, (void*)status, 0);
    return errcode;
}

redConfigT* RedLoadConfig (const char* filepath, int warning) {
    redConfigT *config;
    int errcode= SchemaLoad (filepath, &ConfTopSchema, (void**)&config, warning);
    if (errcode) return NULL;
    return config;  
}

redStatusT* RedLoadStatus (const char* filepath, int warning) {
    redStatusT *status;
    int errcode = SchemaLoad (filepath, &StatusTopSchema, (void**)&status, warning);
    if (errcode) return NULL;
    return  status;  
}

