/*************************************************************************
 File Name: loongson-security.c
 Author: zhuyaliang
 
 Copyright (C) 2022  zhuyaliang https://github.com/zhuyaliang/
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 
 Created Time: 2022年07月07日 星期四 20时02分18秒
************************************************************************/
#include "loongson-security.h"
#include "loongson-utils.h"

struct _LoongsonSecurityPrivate
{
    char *name;
};

G_DEFINE_TYPE_WITH_PRIVATE (LoongsonSecurity, loongson_security, GTK_TYPE_BOX)

static void
loongson_security_fill (LoongsonSecurity *security)
{
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *table;
    g_autoptr(GdkPixbuf) pb = NULL;
    g_autoptr(GdkPixbuf) pb2 = NULL;

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_box_pack_start (GTK_BOX (security), vbox, FALSE, FALSE, 0);
    
    pb = gdk_pixbuf_new_from_file (ICONSDIR"loongson-security.png", NULL);
    pb2 = gdk_pixbuf_scale_simple (pb, 120, 120, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pb2);
    gtk_box_pack_start (GTK_BOX (vbox), image, FALSE, FALSE, 12);
    
    table = grid_widget_new ();
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 6);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("key management"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 0, 1, 1);
    
    label = gtk_label_new (_("What is this"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 0, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Memory Check"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 1, 1, 1);
    
    label = gtk_label_new (_("Single-bit ECC Check"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 1, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Trusted start"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 2, 1, 1);
    
    label = gtk_label_new (_("What is this"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 2, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Firewall"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 3, 1, 1);
    
    label = gtk_label_new (_("Enable"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 3, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("SELinux"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 4, 1, 1);
    
    label = gtk_label_new (_("Disable"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 4, 1, 1);

}

static GObject *
loongson_security_constructor (GType                  type,
                           guint                  n_construct_properties,
                           GObjectConstructParam *construct_properties)
{
    GObject        *obj;
    LoongsonSecurity   *security;

    obj = G_OBJECT_CLASS (loongson_security_parent_class)->constructor (type,
                                      n_construct_properties,
                                      construct_properties);

    security = LOONGSON_SECURITY (obj);
    loongson_security_fill (security);

    return obj;
}

static void
loongson_security_dispose (GObject *object)
{
    LoongsonSecurity *security;

    security = LOONGSON_SECURITY (object);
    g_free (security->priv->name);
    G_OBJECT_CLASS (loongson_security_parent_class)->dispose (object);
}

static void
loongson_security_class_init (LoongsonSecurityClass *klass)
{
    GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->constructor = loongson_security_constructor;
    gobject_class->dispose = loongson_security_dispose;
}

static void
loongson_security_init (LoongsonSecurity *security)
{
    security->priv = loongson_security_get_instance_private (security);
    gtk_orientable_set_orientation (GTK_ORIENTABLE (security), GTK_ORIENTATION_VERTICAL);
    security->priv->name = g_strdup (_("Loongson Security"));
}

const char *loongson_security_get_name (LoongsonSecurity *security)
{
    return security->priv->name;
}

GtkWidget *
loongson_security_new (void)
{
    LoongsonSecurity *security;

    security = g_object_new (LOONGSON_TYPE_SECURITY, NULL);

    return GTK_WIDGET (security);
}
