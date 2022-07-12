#ifndef __LOONGSON_SPEC__
#define __LOONGSON_SPEC__

#include <gtk/gtk.h>
G_BEGIN_DECLS

#define LOONGSON_TYPE_SPEC         (loongson_spec_get_type ())
#define LOONGSON_SPEC(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_SPEC, LoongsonSpec))
#define LOONGSON_SPEC_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k),     LOONGSON_TYPE_SPEC, LoongsonSpecClass))
#define LOONGSON_IS_SPEC(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_SPEC))

typedef struct _LoongsonSpec        LoongsonSpec;
typedef struct _LoongsonSpecClass   LoongsonSpecClass;
typedef struct _LoongsonSpecPrivate LoongsonSpecPrivate;

struct _LoongsonSpec {
    GtkBox                parent_instance;
    LoongsonSpecPrivate  *priv;
};

struct _LoongsonSpecClass {
    GtkBoxClass parent_class;
};

GType           loongson_spec_get_type                (void) G_GNUC_CONST;

GtkWidget      *loongson_spec_new                     (void);

const char     *loongson_spec_get_name                (LoongsonSpec *spec);

G_END_DECLS

#endif
