/*************************************************************************
 File Name: loongson-perf.c
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
 
 Created Time: 2022年07月07日 星期四 20时06分47秒
************************************************************************/
#include "loongson-perf.h"
#include "loongson-utils.h"

struct _LoongsonPerfPrivate
{
    char *name;
};

G_DEFINE_TYPE_WITH_PRIVATE (LoongsonPerf, loongson_perf, GTK_TYPE_BOX)

static void
loongson_perf_fill (LoongsonPerf *perf)
{
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *table;
    g_autoptr(GdkPixbuf) pb = NULL;
    g_autoptr(GdkPixbuf) pb2 = NULL;

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_box_pack_start (GTK_BOX (perf), vbox, FALSE, FALSE, 0);

    pb = gdk_pixbuf_new_from_file (ICONSDIR"loongson-perf.png", NULL);
    pb2 = gdk_pixbuf_scale_simple (pb, 120, 120, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pb2);
    gtk_box_pack_start (GTK_BOX (vbox), image, FALSE, FALSE, 12);

    table = grid_widget_new ();
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 6);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Physical Kernel"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 0, 1, 1);

    label = gtk_label_new (_("4"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 0, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Maximum Memory frequency"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 1, 1, 1);

    label = gtk_label_new (_("2500 HZ"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 1, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Memory channel"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 2, 1, 1);

    label = gtk_label_new (_("8"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 2, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Maximum Frequency"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 3, 1, 1);

    label = gtk_label_new (_("2500 HZ"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 3, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Current Frequency"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 4, 1, 1);

    label = gtk_label_new (_("2500 HZ"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 4, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Maximum memory capacity"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 5, 1, 1);

    label = gtk_label_new (_("128 G"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 5, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Number of threads"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 6, 1, 1);

    label = gtk_label_new ("16");
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 6, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("BogoMIPS"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 7, 1, 1);

    label = gtk_label_new (_("5000"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 7, 1, 1);

    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Address sizes"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 8, 1, 1);

    label = gtk_label_new (_("48 bits physical, 48 bits virtual"));
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 8, 1, 1);

}

static GObject *
loongson_perf_constructor (GType                  type,
                           guint                  n_construct_properties,
                           GObjectConstructParam *construct_properties)
{
    GObject        *obj;
    LoongsonPerf   *perf;

    obj = G_OBJECT_CLASS (loongson_perf_parent_class)->constructor (type,
                                      n_construct_properties,
                                      construct_properties);

    perf = LOONGSON_PERF (obj);
    loongson_perf_fill (perf);

    return obj;
}

static void
loongson_perf_dispose (GObject *object)
{
    LoongsonPerf *perf;

    perf = LOONGSON_PERF (object);
    g_free (perf->priv->name);
    G_OBJECT_CLASS (loongson_perf_parent_class)->dispose (object);
}

static void
loongson_perf_class_init (LoongsonPerfClass *klass)
{
    GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->constructor = loongson_perf_constructor;
    gobject_class->dispose = loongson_perf_dispose;
}

static void
loongson_perf_init (LoongsonPerf *perf)
{
    perf->priv = loongson_perf_get_instance_private (perf);
    gtk_orientable_set_orientation (GTK_ORIENTABLE (perf), GTK_ORIENTATION_VERTICAL);
    perf->priv->name = g_strdup (_("Loongson Perf"));
}

const char *loongson_perf_get_name (LoongsonPerf *perf)
{
    return perf->priv->name;
}

GtkWidget *
loongson_perf_new (void)
{
    LoongsonPerf *perf;

    perf = g_object_new (LOONGSON_TYPE_PERF, NULL);

    return GTK_WIDGET (perf);
}
