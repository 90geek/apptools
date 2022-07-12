#ifndef __LOONGSON_PERF__
#define __LOONGSON_PERF__

#include <gtk/gtk.h>
#include <libintl.h>
#include <locale.h>
#include <glib/gi18n.h>
G_BEGIN_DECLS

#define LOONGSON_TYPE_PERF         (loongson_perf_get_type ())
#define LOONGSON_PERF(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_PERF, LoongsonPerf))
#define LOONGSON_PERF_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k),     LOONGSON_TYPE_PERF, LoongsonPerfClass))
#define LOONGSON_IS_PERF(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_PERF))

typedef struct _LoongsonPerf        LoongsonPerf;
typedef struct _LoongsonPerfClass   LoongsonPerfClass;
typedef struct _LoongsonPerfPrivate LoongsonPerfPrivate;

struct _LoongsonPerf {
    GtkBox                parent_instance;
    LoongsonPerfPrivate  *priv;
};

struct _LoongsonPerfClass {
    GtkBoxClass parent_class;
};

GType           loongson_perf_get_type                (void) G_GNUC_CONST;

GtkWidget      *loongson_perf_new                     (void);

const char     *loongson_perf_get_name                (LoongsonPerf *perf);

G_END_DECLS

#endif
