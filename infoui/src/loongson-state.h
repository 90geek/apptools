#ifndef __LOONGSON_STATE__
#define __LOONGSON_STATE__

#include <gtk/gtk.h>
G_BEGIN_DECLS

#define LOONGSON_TYPE_STATE         (loongson_state_get_type ())
#define LOONGSON_STATE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_STATE, LoongsonState))
#define LOONGSON_STATE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k),     LOONGSON_TYPE_STATE, LoongsonStateClass))
#define LOONGSON_IS_STATE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_STATE))

typedef struct _LoongsonState        LoongsonState;
typedef struct _LoongsonStateClass   LoongsonStateClass;
typedef struct _LoongsonStatePrivate LoongsonStatePrivate;

struct _LoongsonState {
    GtkBox                parent_instance;
    LoongsonStatePrivate  *priv;
};

struct _LoongsonStateClass {
    GtkBoxClass parent_class;
};

GType           loongson_state_get_type                (void) G_GNUC_CONST;

GtkWidget      *loongson_state_new                     (void);

const char     *loongson_state_get_name                (LoongsonState *state);

G_END_DECLS

#endif
