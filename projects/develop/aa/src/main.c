#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wlc/wlc.h>
#include <linux/input.h>

#include "config.h"

#define DEFAULT_CONFIG "~/.config/xxwm"

pid_t statusbar_pid;        // pid of statusbar process
wlc_handle statusbar = 0;   // statusbar view
pid_t desktop_pid;          // pid of desktop app
wlc_handle desktop = 0;     // desktop view

// current view and details of active interactive action
static struct {
    struct {
        wlc_handle view;
        struct wlc_point grab;
        uint32_t edges;
    } action;
} compositor;

// функция, которая запускает интерактивное действие
static bool start_interactive_action(wlc_handle view, const struct wlc_point *origin) {
    // если нет активного окна, выходим
    if (compositor.action.view)
        return false;

    // запоминаем активное окно
    compositor.action.view = view;
    // изначальную точку расположения окна
    compositor.action.grab = *origin;
    // выводит активное окно на первый план
    wlc_view_bring_to_front(view);
    return true;
}

// начинает интерактивное передвижение окна
static void start_interactive_move(wlc_handle view, const struct wlc_point *origin) {
    start_interactive_action(view, origin);
}

// начинает интерактивное изменение размеров окна
static void start_interactive_resize(wlc_handle view, uint32_t edges, const struct wlc_point *origin) {
    const struct wlc_geometry *g;
    // получаем параметры текущего окна
    // и начинаем интерактивное действие
    if (!(g = wlc_view_get_geometry(view)) || !start_interactive_action(view, origin))
        return;

    // определяем, какую грань окна передвигать
    const int32_t halfw = g->origin.x + g->size.w / 2; // координата середины окна по горизонтали
    const int32_t halfh = g->origin.y + g->size.h / 2; // координата середины окна по вертикали
    if (!(compositor.action.edges = edges)) {
        // если начальная точка окна левее середины, перемещаем левую грань
        // если начальная точка окна правее середины, перемещаем правую грань
        // точно так же с верхней и нижней границей
        compositor.action.edges = (origin->x < halfw ? WLC_RESIZE_EDGE_LEFT : (origin->x > halfw ? WLC_RESIZE_EDGE_RIGHT : 0)) |
                                  (origin->y < halfh ? WLC_RESIZE_EDGE_TOP : (origin->y > halfh ? WLC_RESIZE_EDGE_BOTTOM : 0));
    }

    // устанавливаем флаг, что окно в текущий момент меняет свой размер
    wlc_view_set_state(view, WLC_BIT_RESIZING, true);
}

// завершение интерактивного действия
static void stop_interactive_action(void) {
    // если нет активного окна, ничего не делаем
    if (!compositor.action.view)
        return;

    // снимаем флаг изменения размеров окна
    wlc_view_set_state(compositor.action.view, WLC_BIT_RESIZING, false);
    // обнуляем все флаги действий активного окна
    memset(&compositor.action, 0, sizeof(compositor.action));
}

// получить самое верхнее окно по смещению
static wlc_handle get_topmost(wlc_handle output, size_t offset) {
    size_t memb;
    // получаем все окна
    const wlc_handle *views = wlc_output_get_views(output, &memb);
    // возвращаем окно по смещению, или 0 если окон нет
    return (memb > 0 ? views[(memb - 1 + offset) % memb] : 0);
}

// перерисовка всех окон
static void relayout(wlc_handle output) {
    const struct wlc_size *r;
    // получаем разрешение ОМ
    if (!(r = wlc_output_get_virtual_resolution(output)))
        return;

    // получаем самое верхнее окно
    wlc_handle topview = get_topmost(output, 0);
    uint32_t shift = 30;
    // получаем имя верхнего окна
    const char* app = wlc_view_get_title(topview);
    // создаем структуру с параметрами отрисовки окна статусбара
    const struct wlc_geometry gstatus = {
            .origin = {
                    .x = 0,
                    .y = 0
            },
            .size = {
                    .w = r->w,
                    .h = shift
            }
    };
    // получаем тип окна и определяем как его рисовать
    uint32_t viewtype = wlc_view_get_type(topview);
    bool isPopup = (viewtype & WLC_BIT_POPUP) == WLC_BIT_POPUP;
    bool isUnmanaged = (viewtype & WLC_BIT_UNMANAGED) == WLC_BIT_UNMANAGED;
    // если статусбар до сих пор не запущен, значит первое окно это статусбар
    if (statusbar == 0 && wlc_view_get_pid(topview) == statusbar_pid) {
        statusbar = topview;
    // если статусбар запущен, значит перерисовывается другое окно (которое на данный момент активно)
    } else {
        if (wlc_view_get_pid(topview) == desktop_pid) {
            desktop = topview;
        }
        if (isUnmanaged) {
            // если установлен флаг unmanaged, значит нам не надо менять параметры данного окна,
            // т.к. это меню приложения с уже верно заданными параметрами

        } else if (isPopup) {
            // если установлен флаг popup, значит создается всплывающее уведомление
            const struct wlc_geometry* anchor_rect = wlc_view_positioner_get_anchor_rect(topview);
            // получаем размеры окна
            struct wlc_size size_req = *wlc_view_positioner_get_size(topview);
            if ((size_req.w <= 0) || (size_req.h <= 0)) {
                const struct wlc_geometry* current = wlc_view_get_geometry(topview);
                size_req = current->size;
            }
            // задаем размеры окна такие же, как и были при создании
            struct wlc_geometry gpopup = {
                    .origin = anchor_rect->origin,
                    .size = size_req
            };
            // если есть родительское окно, смещаем popup относительно его начала
            wlc_handle parent = wlc_view_get_parent(topview);
            if (parent) {
                const struct wlc_geometry* parent_geometry = wlc_view_get_geometry(parent);
                gpopup.origin.x += parent_geometry->origin.x;
                gpopup.origin.y += parent_geometry->origin.y;
            }
            // перерисовываем окно
            wlc_view_set_geometry(topview, 0, &gpopup);

        } else {
            // иначе создается обычное окно
            // задаем параметры отрисовки активного окна
            const struct wlc_geometry gview = {
                    .origin = {
                            .x = 0,
                            .y = shift
                    },
                    .size = {
                            .w = r->w,
                            .h = r->h - shift
                    }
            };
            // перерисовываем активное окно
            wlc_view_set_geometry(topview, 0, &gview);
        }
    }
    // на всякий случай перерисовываем статусбар (т.к. могло поменяться разрешение экрана)
    if (statusbar != 0) wlc_view_set_geometry(statusbar, 0, &gstatus);
}

// смена разрешения окна ОМ
static void output_resolution(wlc_handle output, const struct wlc_size *from, const struct wlc_size *to) {
    (void)from, (void)to;
    relayout(output);
}

// обработка нового созданного приложения
static bool view_created(wlc_handle view) {
    // устанавливаем флаги отображения окна (по-умолчанию - 1, т.е. рисовать)
    wlc_view_set_mask(view, wlc_output_get_mask(wlc_view_get_output(view)));
    // переносим новое окно на первый план
    wlc_view_bring_to_front(view);
    // устанавливаем новое окно активным
    wlc_view_focus(view);
    // указываем, чтобы новое окно рисовалось во весь экран
    wlc_view_set_state(view, WLC_BIT_FULLSCREEN, true);
    // перерисовываем все окна
    relayout(wlc_view_get_output(view));
    return true;
}

// обработка завершившегося приложения
static void view_destroyed(wlc_handle view) {
    // получаем самое верхнее окно и выносим его на первый план
    wlc_view_focus(get_topmost(wlc_view_get_output(view), 0));
    // перерисовываем все окна
    relayout(wlc_view_get_output(view));
}

// установить окно активным
static void view_focus(wlc_handle view, bool focus) {
    // устанавливаем окну флаг активнсти
    wlc_view_set_state(view, WLC_BIT_ACTIVATED, focus);
}

// запрос на перемещение окна
static void view_request_move(wlc_handle view, const struct wlc_point *origin) {
    start_interactive_move(view, origin);
}

// запрос на изменение размеров окна
static void view_request_resize(wlc_handle view, uint32_t edges, const struct wlc_point *origin) {
    start_interactive_resize(view, edges, origin);
}

// запрос на изменение отображения окна
static void view_request_geometry(wlc_handle view, const struct wlc_geometry *g) {
    (void)view, (void)g;
    // заглушка, не позволяющая изменить окно
}

// обработка событий клавиатуры
static bool keyboard_key(wlc_handle view, uint32_t time,
                         const struct wlc_modifiers *modifiers, uint32_t key, enum wlc_key_state state) {
    (void)time, (void)key;
    // получаем считанный с клавиатуры символ
    const uint32_t sym = wlc_keyboard_get_keysym_for_key(key, NULL);

    // если есть активное окно
    if (view) {
        // CTRL+q -- закрытие окна
        if (modifiers->mods & WLC_BIT_MOD_CTRL && sym == XKB_KEY_q) {
            // close the window, if it's not system view
            if (state == WLC_KEY_STATE_PRESSED && view != statusbar && view != desktop) {
                wlc_view_close(view);
            }
            return true;
        // CTRL+стрелка вниз -- перелистывание окон
        } else if (modifiers->mods & WLC_BIT_MOD_CTRL && sym == XKB_KEY_Down) {
            if (state == WLC_KEY_STATE_PRESSED) {
                wlc_view_send_to_back(view); // отправляем текущее окно на задний план
                wlc_view_focus(get_topmost(wlc_view_get_output(view), 0)); // устанавливаем новое верхнее окно активным
            }
            return true;
        }
    }

    // CTRL+Escape -- завершение работы
    if (modifiers->mods & WLC_BIT_MOD_CTRL && sym == XKB_KEY_Escape) {
        if (state == WLC_KEY_STATE_PRESSED) {
            wlc_terminate();
        }
        return true;
    // CTRL+Enter -- запускаем терминал
    } else if (modifiers->mods & WLC_BIT_MOD_CTRL && sym == XKB_KEY_Return) {
        if (state == WLC_KEY_STATE_PRESSED) {
            char *terminal = (getenv("TERMINAL") ? getenv("TERMINAL") : "weston-terminal");
            wlc_exec(terminal, (char *const[]){ terminal, NULL });
        }
        return true;
    }

    return false;
}

// обработка нажатий кнопок мыши
static bool pointer_button(wlc_handle view, uint32_t time, const struct wlc_modifiers *modifiers,
                           uint32_t button, enum wlc_button_state state, const struct wlc_point *position) {
    (void)button, (void)time, (void)modifiers;

    // если кнопка нажата, то начинаем интерактивное действие
    if (state == WLC_BUTTON_STATE_PRESSED) {
        wlc_view_focus(view);
        if (view) {
            // CTRL+левая кнопка -- передвигаем окно
            if (modifiers->mods & WLC_BIT_MOD_CTRL && button == BTN_LEFT)
                start_interactive_move(view, position);
            // CTRL+правая кнопка -- изменяем размеры окна
            if (modifiers->mods & WLC_BIT_MOD_CTRL && button == BTN_RIGHT)
                start_interactive_resize(view, 0, position);
        }
    // иначе завершаем интерактивное действие
    } else {
        stop_interactive_action();
    }

    return (compositor.action.view ? true : false);
    //return false;
}

// обарботка движения мыши
static bool pointer_motion(wlc_handle handle, uint32_t time, const struct wlc_point *position) {
    (void)handle, (void)time;

    // если есть активное окно
    if (compositor.action.view) {
        // определяем координаты мышки относительно окна
        const int32_t dx = position->x - compositor.action.grab.x;
        const int32_t dy = position->y - compositor.action.grab.y;
        struct wlc_geometry g = *wlc_view_get_geometry(compositor.action.view);

        // если есть запросы на перерисовку границ окна
        if (compositor.action.edges) {
            const struct wlc_size min = { 80, 40 }; // минимально допустиыме размеры окна

            struct wlc_geometry n = g;
            if (compositor.action.edges & WLC_RESIZE_EDGE_LEFT) {
                n.size.w -= dx;
                n.origin.x += dx;
            } else if (compositor.action.edges & WLC_RESIZE_EDGE_RIGHT) {
                n.size.w += dx;
            }

            if (compositor.action.edges & WLC_RESIZE_EDGE_TOP) {
                n.size.h -= dy;
                n.origin.y += dy;
            } else if (compositor.action.edges & WLC_RESIZE_EDGE_BOTTOM) {
                n.size.h += dy;
            }

            if (n.size.w >= min.w) {
                g.origin.x = n.origin.x;
                g.size.w = n.size.w;
            }

            if (n.size.h >= min.h) {
                g.origin.y = n.origin.y;
                g.size.h = n.size.h;
            }

            // устанавливаем новые размеры окна
            wlc_view_set_geometry(compositor.action.view, compositor.action.edges, &g);
        // если нет запросов на изменение размеров окна, значит мы его перемещаем
        } else {
            g.origin.x += dx;
            g.origin.y += dy;
            wlc_view_set_geometry(compositor.action.view, 0, &g);
        }

        // запоминаем текущую позицию мыши
        compositor.action.grab = *position;
    }

    // Устанавливаем координаты мышки и возвращаем управление композитору
    wlc_pointer_set_position(position);
    return (compositor.action.view ? true : false);
}

// функция логирования
static void cb_log(enum wlc_log_type type, const char *str) {
    (void)type;
    printf("%s\n", str); // выводим все в стандартный поток вывода
}

int main(int argc, char** argv) {
    // parse input arguments
    char* config = DEFAULT_CONFIG;
    if (argc < 2) {
        printf("No config file specified, using default: %s\n", config);
    } else {
        config = argv[1];
        printf("Using config file: %s\n", config);
    }
    // parse config
    init_config(config);


    // устанавливаем функцию-логгер
    wlc_log_set_handler(cb_log);

    // устанавливаем функцию, которая отвечает за перерисовку всех окон при смене разрешения
    wlc_set_output_resolution_cb(output_resolution);
    // устанавливаем функцию, которая отвечает за обработку запущенный приложений
    wlc_set_view_created_cb(view_created);
    // устанавливаем функцию, которая отвечает за обработку завершившихся приложений
    wlc_set_view_destroyed_cb(view_destroyed);
    // устанавливаем функцию, которая отвечает за смену фокуса между окнами (выбирает активное приложение)
    wlc_set_view_focus_cb(view_focus);
    // устанавливаем функцию, которая отвечает за передвижение окна ОМ по экрану
    wlc_set_view_request_move_cb(view_request_move);
    // устанавливаем функцию, которая отвечает за смену размеров окна ОМ
    wlc_set_view_request_resize_cb(view_request_resize);
    // устанавливаем функцию, которая отвечает за смену изменения отображения окна ОМ
    wlc_set_view_request_geometry_cb(view_request_geometry);
    // устанавливаем функцию-обработчик нажатий на клавиатуру
    wlc_set_keyboard_key_cb(keyboard_key);
    // устанавливаем функцию, которая отвечает за обработку нажатия кнопок мыши
    wlc_set_pointer_button_cb(pointer_button);
    // устанавливаем функцию, которая отвечает за передвижение мыши
    wlc_set_pointer_motion_cb(pointer_motion);

    // инициализируем композитор
    if (!wlc_init())
        return EXIT_FAILURE;

    // spawning process that starts statusbar
    statusbar_pid = fork();
    if (statusbar_pid < 0) {
        printf("Startup launch failure\n");
        return EXIT_FAILURE;

    } else if (statusbar_pid == 0) {
        const char *statusbar = get_statusbar();
        printf("Running statusbar %s\n", statusbar);
        execv(statusbar, (char *const[]) {statusbar, NULL});

    } else {
        // spawning process that starts desktop
        desktop_pid = fork();
        if (desktop_pid < 0) {
            printf("Startup launch failure\n");
            return EXIT_FAILURE;

        } else if (desktop_pid == 0) {
            // sleep for some time, wait until the WM actually starts
            usleep(250000);
            const char *desktop = get_desktop();
            printf("Running desktop %s\n", desktop);
            execv(desktop, (char *const[]) {desktop, NULL});

        }

        printf("Running WM\n");
        wlc_run();
    }

    return EXIT_SUCCESS;
}
