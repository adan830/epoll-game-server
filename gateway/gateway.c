/*  
    Copyright (C) <2013>  <jjchen.lian@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "engine.h"
#include "engine_manager.h"
#include "signal_h.h"
    
#define DRIVE_RECV_HEAD 0
#define DRIVE_RECV_DATA 1
#define DRIVE_FD_CLOSE 2
#define DRIVE_FD_ERROR 3

#define MAX_EVENTS 500

extern engine_t * engine;

int main() {

    signal(SIGINT, sig_process);
    signal(SIGPIPE, sig_process);

    engine = new_engine();
    start_engine(engine);
    return 0;
}