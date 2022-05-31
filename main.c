#define WINVER 0x0500
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GLOBAL_PAUSE_SEC (120)

const char * window_name = NULL;

static void _send_input(INPUT * in, WORD wVk, DWORD dwFlags) {
    in->ki.wVk     = wVk;
    in->ki.dwFlags = dwFlags; 
    SendInput(1, in, sizeof(INPUT));
}

static void _send_ctrls(void) {
    INPUT in = { 0 };
    in.type = INPUT_KEYBOARD;

    _send_input(&in, VK_CONTROL, 0);
    _send_input(&in, 'S',        0);
    _send_input(&in, 'S',        KEYEVENTF_KEYUP);
    _send_input(&in, VK_CONTROL, KEYEVENTF_KEYUP);
}

static int _check_title(HWND w) {
    char title[256];
    GetWindowText(w, title, 256);

    return (strstr(title, window_name) == 0);
}

static void _save_work(void) {
    HWND win = GetForegroundWindow();
    if (!win)
        return;

    if (_check_title(win))
        return;

    _send_ctrls();
}

static void _hide(void) {
    HWND cwin = GetConsoleWindow(); 
    ShowWindow(cwin, SW_MINIMIZE);
    ShowWindow(cwin, SW_HIDE); 
}

int main(int argc, char * argv[]) {
    _hide();
    
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments. Expecting window name part");
        return EXIT_FAILURE;
    }   
    
    window_name = argv[1];

    while (1) {
        _save_work();
        sleep(GLOBAL_PAUSE_SEC);
    }
}
