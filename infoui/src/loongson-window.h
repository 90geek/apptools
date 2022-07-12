#ifndef __LOONGSON_WINDOW__
#define __LOONGSON_WINDOW__

#include "loongson-utils.h"
G_BEGIN_DECLS

#define LOONGSON_TYPE_WINDOW         (loongson_window_get_type ())
#define LOONGSON_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_WINDOW, LoongsonWindow))
#define LOONGSON_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k),     LOONGSON_TYPE_WINDOW, LoongsonWindowClass))
#define LOONGSON_IS_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_WINDOW))

typedef struct _LoongsonWindow        LoongsonWindow;
typedef struct _LoongsonWindowClass   LoongsonWindowClass;
typedef struct _LoongsonWindowPrivate LoongsonWindowPrivate;

struct _LoongsonWindow {
    GtkWindow               parent_instance;
    LoongsonWindowPrivate  *priv;
};

struct _LoongsonWindowClass {
    GtkWindowClass parent_class;
};

GType              loongson_window_get_type                (void) G_GNUC_CONST;

GtkWidget         *loongson_window_new                     (void);

G_END_DECLS

#endif
