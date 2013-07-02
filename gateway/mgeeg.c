#include "global.h"

void start_game_server();

int main(  )  
    {  
        // int thePipe[ 2 ],ret;
        // log_unit_data *m_log_unit_data = (log_unit_data *)malloc(sizeof(log_unit_data*)); 
        // if( pipe( thePipe ) == 0 )  
        //     {  
                start_game_server();
        //         if( fork() == 0 )  
        //             {  
        //                 ret = read( thePipe[ 0 ], m_log_unit_data, sizeof(log_unit_data*) );
        //                 log_print(m_log_unit_data->level, m_log_unit_data->buf);  
                        
        //             }  
        //     }  
        // close( thePipe[ 0 ] );  
        // close( thePipe[ 1 ] );  
          
        return 0;  
          
    }  
