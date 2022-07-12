#ifndef __LOONGSON_MAINBOARD__
#define __LOONGSON_MAINBOARD__

#include <gtk/gtk.h>
G_BEGIN_DECLS

#define LOONGSON_TYPE_MAINBOARD         (loongson_mainboard_get_type ())
#define LOONGSON_MAINBOARD(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_MAINBOARD, LoongsonMainboard))
#define LOONGSON_MAINBOARD_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k),     LOONGSON_TYPE_MAINBOARD, LoongsonMainboardClass))
#define LOONGSON_IS_MAINBOARD(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_MAINBOARD))

typedef struct _LoongsonMainboard        LoongsonMainboard;
typedef struct _LoongsonMainboardClass   LoongsonMainboardClass;
typedef struct _LoongsonMainboardPrivate LoongsonMainboardPrivate;

struct _LoongsonMainboard {
    GtkBox                parent_instance;
    LoongsonMainboardPrivate  *priv;
};

struct _LoongsonMainboardClass {
    GtkBoxClass parent_class;
};

GType           loongson_mainboard_get_type                (void) G_GNUC_CONST;

GtkWidget      *loongson_mainboard_new                     (void);

const char     *loongson_mainboard_get_name                (LoongsonMainboard *mainboard);

G_END_DECLS

#endif
