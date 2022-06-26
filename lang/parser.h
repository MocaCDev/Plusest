#ifndef PARSER
#define PARSER
#include "lexer.h"
#include "read.h"
#include "wrappers.h"
#include "util.h"
#include "rust_state.h"
#include <stdbool.h>

#define ERR(msg, ...) {\
    fprintf(stderr, msg, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);       \
}

typedef struct CreateInformation
{
    char *keyword_name;
    // All wrapper IDS
    enum {
        DEFAULT = 0x0,
        W_VarWrapper = 0x10
    } wrapper_id;
    //_PreAssigns pa;
    // ToDo: More fields.
} _CreateInfo;

typedef struct Parser
{
    _Token *token;
    _CreateInfo **ci;
    int amnt;
    _Lexer *lex;
    int amnt_of_types;
    _CreateInfo *current_config_info;
} _Parser;

#include "runner.h"

void parse_create(_Parser *p)
{
    p->token = get_token(p->lex);

    if(p->token->id == l_par)
    {
        p->token = get_token(p->lex);

        p->ci[p->amnt]->keyword_name = p->token->token_val;
        p->amnt++;
        p->ci[p->amnt] = (_CreateInfo *)calloc(1, sizeof(*p->ci[p->amnt]));

        p->token = get_token(p->lex);

        if(p->token->id == r_par)
        {
            p->token = get_token(p->lex);
            if(p->token->id == semi_colon)
            {
               // p->token = get_token(p->lex);
                return;
            }

            ERR("Expected `;`")
        }

        ERR("Expected `)`.")
    }
}

void parse_set(_Parser *p)
{
    p->token = get_token(p->lex);

    if(p->token->id == as)
    {
        p->token = get_token(p->lex);

        switch(p->token->id)
        {
            case WRAPPER_VarWrapper: {
                p->current_config_info->wrapper_id = W_VarWrapper;
                break;
            }
            default: break;
        }

        p->token = get_token(p->lex);
        return;
    }

    ERR("Expected `as` following `set`.")
}

void parse_configure(_Parser *p)
{
    p->token = get_token(p->lex);

    char is_valid_keyword = 0;
    int index = -1;

    if(p->token->id == l_par)
    {
        p->token = get_token(p->lex);
    
        for(int i = 0; i < p->amnt; i++)
            {
                if(strcmp(p->token->token_val, p->ci[i]->keyword_name)==0)
                {
                    index = i;
                    is_valid_keyword = 1;
                    break;
                }
            }

        if(is_valid_keyword == 1)
        {
            p->current_config_info = p->ci[index];
            p->token = get_token(p->lex);

            if(p->token->id == r_par)
            {
                p->token = get_token(p->lex);

                if(p->token->id == colon)
                {
                    // Code for custom langauge configuration
                    return;
                }

                if(p->token->id == l_sqrbrack)
                {
                    p->token = get_token(p->lex);
                    
                    switch(p->token->id)
                    {
                        case set: {
                            parse_set(p);
                            break;
                        }
                    }
                    p->token = get_token(p->lex);
                
                    return;
                }

                ERR("Expected `{`.")
            }
            
        }
        ERR("Expected keyword made from `CREATE` to be passed as parameter for `CONFIGURE`.")
    }
    ERR("Expected `(`.")
}

void parse_target_os(_Parser *p)
{
    p->token = get_token(p->lex);

    if(p->token->id == colon)
    {
redo:
        if(config_i->OS_INFO.amnt_of_types >= MAX_OS)
            ERR("There are only %d supported operating systems.", MAX_OS)
            
        p->token = get_token(p->lex);

        switch(p->token->id)
            {
                case OS_LINUX:
                    {
                        config_i->OS_INFO.types[config_i->OS_INFO.amnt_of_types] = T_LINUX;
                        p->amnt_of_types++;
                        config_i->OS_INFO.amnt_of_types++;

                        p->token = get_token(p->lex);
                        if(p->token->id != comma)
                            goto end;

                        break;
                    }
                case OS_WINDOWS:
                    {
                        config_i->OS_INFO.types[config_i->OS_INFO.amnt_of_types] = T_WINDOWS;
                        p->amnt_of_types++;
                        config_i->OS_INFO.amnt_of_types++;

                        p->token = get_token(p->lex);
                        if(p->token->id != comma)
                            goto end;

                        break;
                    }
            }
        goto redo;

        p->token = get_token(p->lex);
end:
        return;
    }

    ERR("Expected `:`.")
}

_Parser *run_parser(_Lexer *lex);

void parse_syntax(_Parser *p)
{
    p->token = get_token(p->lex);

    if(p->token->id == colon)
    {
        p->token = get_token(p->lex);
        if(p->token->id == string)
        {
            {
                /* Mini lexer/parser for separate file. */
                char *data = read_file(p->token->token_val);
                _Lexer *lex = init_lexer(data);
                _Parser *pars = run_parser(lex);

                p->ci = pars->ci;
                config_i->OS_INFO.amnt_of_types = pars->amnt_of_types;
                p->amnt = pars->amnt;
            }
            return;
        }
        ERR("Expected string.")
    }
    ERR("Colon is required after `syntax`.")
}

void parse_hashtag(_Parser *p)
{
    p->token = get_token(p->lex);

    switch(p->token->id)
        {
            case PA_GROUNDED: {
                p->token = get_token(p->lex);
                p->token = get_token(p->lex);

                switch(p->token->id)
                    {
                        case TARGET_OS: {
                            config_i->OS_INFO.pa.is_grounded = true;
                            break;
                        }
                        default: break;
                    }
                break;
            }
            case PA_OVERRIDED: {
                break;
            }
            default: break;
        }
    
    p->token = get_token(p->lex);
    return;
}

_Parser *main_parser(_Parser *p)
{
    if(config_i->OS_INFO.amnt_of_types > 0)
    {
        bool targets_linux = false;
        bool targets_windows = false;
        
        for(int i = 0; i < config_i->OS_INFO.amnt_of_types; i++)
            {
                switch(config_i->OS_INFO.types[i])
                    {
                        case T_LINUX: {
                            targets_linux = true;
                            break;
                        }
                        case T_WINDOWS: {
                            targets_windows = true;
                            break;
                        }
                        default: break;
                    }
            }

        SetupInfo si;
        if(targets_linux && targets_windows)
        {
            // Move the checking to the runner?
            #if !defined(LINUX) && !defined(WINDOWS)
            ERR("Expected Linux or Windows machine.")
            #endif

            #if defined(LINUX)
            config_i->OS_INFO.targetting_OS = T_LINUX;
            #endif

            #if defined(WINDOWS)
            config_i->OS_INFO.targetting_OS = T_WINDOWS;
            #endif

            si = init_SetupInfo(T_WINDOWS_AND_LINUX);

            goto continue_main_parser;
        } 

        if(targets_linux)
        {
            // Move the checking to the runner?
            #ifndef LINUX
            ERR("Expected Linux system")
            #endif

            #ifdef LINUX
            config_i->OS_INFO.targetting_OS = T_LINUX;
            #endif

            si = init_SetupInfo(T_LINUX_ONLY);

            goto continue_main_parser;
        }

        if(targets_windows)
        {
            // Move the checking to the runner?
            #ifndef WINDOWS
            ERR("Expected Windows system")
            #endif

            #ifdef WINDOWS
            config_i->OS_INFO.targetting_OS = T_WINDOWS;
            #endif

            si = init_SetupInfo(T_WINDOWS_ONLY);

            goto continue_main_parser;
        }
    }

continue_main_parser:

    return p;
}

_Parser *run_parser(_Lexer *lex)
{
    _Parser *p = (_Parser *)calloc(1, sizeof(*p));
    config_i = (ConfigInfo *)calloc(1, sizeof(*config_i));
    
    p->token = get_token(lex);
    p->lex = lex;
    p->amnt = 0;
    config_i->OS_INFO.amnt_of_types = 0;
    p->ci = (_CreateInfo **)calloc(1, sizeof(*p->ci));
    p->ci[p->amnt] = (_CreateInfo *)calloc(1, sizeof(*p->ci[p->amnt]));
    
    while(p->token->id != EndOF)
        {
            switch(p->token->id)
                {
                    case CREATE: {
                        parse_create(p);
                        break;
                    }
                    case TARGET_OS: {
                        parse_target_os(p);
                        break;
                    }
                    case CONFIGURE: {
                        parse_configure(p);
                        break;
                    }
                    case hashtag: {
                        parse_hashtag(p);
                        break;
                    }
                    case set: {
                        parse_set(p);
                        break;
                    }
                    case syntax: {
                        parse_syntax(p);

                        // We should(hopefully) be in the primary file. Just continue parsing afterwards.
                        p = main_parser(p);
                        break;
                    }
                    case EndOF: goto end;
                    default: goto end;
                }
            p->token = get_token(p->lex);
        }
end:
    return p;
}

#endif