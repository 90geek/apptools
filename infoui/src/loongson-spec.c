/*************************************************************************
 File Name: loongson-spec.c
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
 
 Created Time: 2022年07月07日 星期四 18时01分38秒
************************************************************************/
#include "loongson-spec.h"
#include "loongson-utils.h"

struct _LoongsonSpecPrivate
{
    char *name;
};

G_DEFINE_TYPE_WITH_PRIVATE (LoongsonSpec, loongson_spec, GTK_TYPE_BOX)

static void
loongson_spec_fill (LoongsonSpec *spec)
{
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *table;
    g_autoptr(GdkPixbuf) pb = NULL;
    g_autoptr(GdkPixbuf) pb2 = NULL;

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_box_pack_start (GTK_BOX (spec), vbox, FALSE, FALSE, 0);
    
    pb = gdk_pixbuf_new_from_file (ICONSDIR"loongson-spec.png", NULL);
    pb2 = gdk_pixbuf_scale_simple (pb, 150, 106, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pb2);
    gtk_box_pack_start (GTK_BOX (vbox), image, FALSE, FALSE, 0);

    label = gtk_label_new (NULL);
    set_lable_style (label, "black", 13, "LoongArch64", TRUE);
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    
    table = grid_widget_new ();
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 6);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("byte order"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 0, 1, 1);
    
    label = gtk_label_new (_("Little Endian"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 0, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("CPU Series"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 1, 1, 1);
    
    label = gtk_label_new (_("Loongson-64bit"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 1, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("CPU Model Name"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 2, 1, 1);
    
    label = gtk_label_new (_("Loongson-3A5000"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 2, 1, 1);
 
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Process Technology"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 3, 1, 1);
    
    label = gtk_label_new (_("28 nm"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 3, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Power Dissipation"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 4, 1, 1);
    
    label = gtk_label_new (_("28 W"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 4, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Junction Temperature "), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 5, 1, 1);
    
    label = gtk_label_new (_("28 C"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 5, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Packaging Method"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 6, 1, 1);
    
    label = gtk_label_new (_("PGA"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 6, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Size"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 7, 1, 1);
    
    label = gtk_label_new (_("28 X 28 X 28"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 7, 1, 1);

}

static GObject *
loongson_spec_constructor (GType                  type,
                           guint                  n_construct_properties,
                           GObjectConstructParam *construct_properties)
{
    GObject        *obj;
    LoongsonSpec   *spec;

    obj = G_OBJECT_CLASS (loongson_spec_parent_class)->constructor (type,
                                      n_construct_properties,
                                      construct_properties);

    spec = LOONGSON_SPEC (obj);
    loongson_spec_fill (spec);

    return obj;
}

static void
loongson_spec_dispose (GObject *object)
{
    LoongsonSpec *spec;

    spec = LOONGSON_SPEC (object);
    g_free (spec->priv->name);
    G_OBJECT_CLASS (loongson_spec_parent_class)->dispose (object);
}

static void
loongson_spec_class_init (LoongsonSpecClass *klass)
{
    GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->constructor = loongson_spec_constructor;
    gobject_class->dispose = loongson_spec_dispose;
}

static void
loongson_spec_init (LoongsonSpec *spec)
{
    spec->priv = loongson_spec_get_instance_private (spec);
    gtk_orientable_set_orientation (GTK_ORIENTABLE (spec), GTK_ORIENTATION_VERTICAL);
    spec->priv->name = g_strdup (_("Loongson Specifications"));
}

const char *loongson_spec_get_name (LoongsonSpec *spec)
{
    return spec->priv->name;
}

GtkWidget *
loongson_spec_new (void)
{
    LoongsonSpec *spec;

    spec = g_object_new (LOONGSON_TYPE_SPEC, NULL);

    return GTK_WIDGET (spec);
}
