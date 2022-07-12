#ifndef __LOONGSON_SECURITY__
#define __LOONGSON_SECURITY__

#include <gtk/gtk.h>
G_BEGIN_DECLS

#define LOONGSON_TYPE_SECURITY         (loongson_security_get_type ())
#define LOONGSON_SECURITY(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), LOONGSON_TYPE_SECURITY, LoongsonSecurity))
#define LOONGSON_SECURITY_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k),     LOONGSON_TYPE_SECURITY, LoongsonSecurityClass))
#define LOONGSON_IS_SECURITY(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), LOONGSON_TYPE_SECURITY))

typedef struct _LoongsonSecurity        LoongsonSecurity;
typedef struct _LoongsonSecurityClass   LoongsonSecurityClass;
typedef struct _LoongsonSecurityPrivate LoongsonSecurityPrivate;

struct _LoongsonSecurity {
    GtkBox                parent_instance;
    LoongsonSecurityPrivate  *priv;
};

struct _LoongsonSecurityClass {
    GtkBoxClass parent_class;
};

GType           loongson_security_get_type                (void) G_GNUC_CONST;

GtkWidget      *loongson_security_new                     (void);

const char     *loongson_security_get_name                (LoongsonSecurity *security);

G_END_DECLS

#endif
