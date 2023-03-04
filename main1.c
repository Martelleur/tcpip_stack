// Include header file for CLI lib.
#include "CommandParser/cmdtlv.h"
#include "CommandParser/libcli.h"


#define CMDODE_SHOW_NODE 1
#define CMDODE_SHOW_NODE_LOOPBACK 2

#define CMDODE_SHOW_SPORT 3


int
node_callback_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable){
    printf("%s() is called ...\n", __FUNCTION__);
    return 0;
}

int
validate_node_name(char *value){
    printf("%s() is called with value = %s\n", __FUNCTION__, value);
    return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}

int
sport_callback_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable){
    printf("%s() is called ...\n", __FUNCTION__);
    return 0;
}

int
validate_sport_name(char *value){
    printf("%s() is called with value = %s\n", __FUNCTION__, value);
    return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}


int
main(int argc, char **argv){

    // Init default hooks etc. 
    init_libcli();
    
    // Import the supported hooks for access.
    // A separated command tree for each hook.
    param_t *show   = libcli_get_show_hook();
    param_t *debug  = libcli_get_debug_hook();
    param_t *config = libcli_get_config_hook();
    param_t *clear  = libcli_get_clear_hook();
    param_t *run    = libcli_get_run_hook();


    /* Implement MY CMD */
    {
        /*show node*/
        static param_t node;    /*Get the param_t variable, either a static memory or heap memory, not stack memory*/
        init_param(&node,       /*Address of the current param*/ 
                CMD,            /*CMD for command param, LEAF for leaf param*/
                "node",         /*Name of the param, this string is what is displayed in command line*/
                0,              /*callback : pointer to application function. Null in this case since 'show node' is not a complete command*/
                0,              /*Applicable only for LEAF params. Always NULL for CMD param*/
                INVALID,        /*Always INVALID for CMD params*/
                0,              /*Always NULL for CMD params*/
                "Help : node"); /*Help String*/

        libcli_register_param(show, &node); /*Add node param as suboption of show param*/
    
        {
            /*show node <node-name>*/
            static param_t node_name;   /*Get the param_t variable, either a static memory or heap memory, not stack memory*/
            init_param(&node_name,      /*Address of the current param*/
                      LEAF,             /*CMD for command param, LEAF for leaf param. Since it is a leaf param which takes node names, hence pass LEAF*/
                      0,                /*Always NULL for LEAF param*/
                      node_callback_handler, /*Since this is complete command, it should invoke application routine. Pass the pointer to that routine here.*/
                      validate_node_name,    /*Optional : can be NULL. This is also application specific routine, and perform validation test to the value entered by the user for this leaf param. If validation pass, then only node_callback_handler routine is invoked*/
                      STRING,               /*leaf param value type. Node name is string, hence pass STRING*/
                      "node-name",          /*Applicable only for LEAF param. Give some name to leaf-params. It is this string that we will parse in application code to find the value passed by the user*/
                      "Help : Node name");  /*Help String*/
            libcli_register_param(&node, &node_name);   /*Add node_name leaf param as suboption of node param. Note that: show --> node --> node_name has been chained*/
            /*The below API should be called for param upto which the command is supposed to invoke application callback rouine. 
             * The CMDODE_SHOW_NODE code is sent to application using which we find which command was triggered, and accordingly what 
             * are expected leaf params we need to parse. More on this ater.*/
            set_param_cmd_code(&node_name, CMDODE_SHOW_NODE);

            /*Implementing CMD2*/
        }
    }
    
    {
        /*show sport*/
        static param_t sport;    /*Get the param_t variable, either a static memory or heap memory, not stack memory*/
        init_param(&sport,       /*Address of the current param*/ 
                CMD,            /*CMD for command param, LEAF for leaf param*/
                "sport",         /*Name of the param, this string is what is displayed in command line*/
                0,              /*callback : pointer to application function. Null in this case since 'show node' is not a complete command*/
                0,              /*Applicable only for LEAF params. Always NULL for CMD param*/
                INVALID,        /*Always INVALID for CMD params*/
                0,              /*Always NULL for CMD params*/
                "Help : Enter a sport"); /*Help String*/

        libcli_register_param(show, &sport); /*Add node param as suboption of show param*/

        {
            /*show sport <sport-name>*/
            static param_t sport_name;   /*Get the param_t variable, either a static memory or heap memory, not stack memory*/
            init_param(&sport_name,      /*Address of the current param*/
                      LEAF,             /*CMD for command param, LEAF for leaf param. Since it is a leaf param which takes node names, hence pass LEAF*/
                      0,                /*Always NULL for LEAF param*/
                      sport_callback_handler, /*Since this is complete command, it should invoke application routine. Pass the pointer to that routine here.*/
                      validate_sport_name,    /*Optional : can be NULL. This is also application specific routine, and perform validation test to the value entered by the user for this leaf param. If validation pass, then only node_callback_handler routine is invoked*/
                      STRING,               /*leaf param value type. Node name is string, hence pass STRING*/
                      "sport-name",          /*Applicable only for LEAF param. Give some name to leaf-params. It is this string that we will parse in application code to find the value passed by the user*/
                      "Help : Enter a sport after show sport!");  /*Help String*/
            libcli_register_param(&sport, &sport_name);   /*Add node_name leaf param as suboption of node param. Note that: show --> node --> node_name has been chained*/
            /*The below API should be called for param upto which the command is supposed to invoke application callback rouine. 
             * The CMDODE_SHOW_NODE code is sent to application using which we find which command was triggered, and accordingly what 
             * are expected leaf params we need to parse. More on this ater.*/
            set_param_cmd_code(&sport_name, CMDODE_SHOW_SPORT);

            /*Implementing CMD2*/
        }
    }


    // Invoke API
    support_cmd_negation(config);
    /*Do not add any param in config command tree after above line*/
    
    // START REPL   
    start_shell();
    return 0;
}
