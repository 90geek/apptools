/*************************************************************************
 File Name: loongson-state.c
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
 
 Created Time: 2022年07月07日 星期四 19时57分02秒
************************************************************************/
#include "loongson-state.h"
#include "loongson-utils.h"

struct _LoongsonStatePrivate
{
    char *name;
};

G_DEFINE_TYPE_WITH_PRIVATE (LoongsonState, loongson_state, GTK_TYPE_BOX)

static void
loongson_state_fill (LoongsonState *state)
{
    GtkWidget *vbox;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *table;
    g_autoptr(GdkPixbuf) pb = NULL;
    g_autoptr(GdkPixbuf) pb2 = NULL;

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_box_pack_start (GTK_BOX (state), vbox, FALSE, FALSE, 0);
    
    pb = gdk_pixbuf_new_from_file (ICONSDIR"loongson-state.png", NULL);
    pb2 = gdk_pixbuf_scale_simple (pb, 120, 120, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pb2);
    gtk_box_pack_start (GTK_BOX (vbox), image, FALSE, FALSE, 12);
    
    table = grid_widget_new ();
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 6);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Temperature"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 0, 1, 1);
    
    label = gtk_label_new (NULL);
    set_lable_style (label, "red", 12, _("80"), TRUE);
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 0, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Fan speed"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 1, 1, 1);
    
    label = gtk_label_new (NULL);
    set_lable_style (label, "green", 12, _("30%"), TRUE);
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 1, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Memory Usage"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 2, 1, 1);
    
    label = gtk_label_new (NULL);
    set_lable_style (label, "yellow", 12, _("60%"), TRUE);
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 2, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("CPU utilization"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 3, 1, 1);
    
    label = gtk_label_new (NULL);
    set_lable_style (label, "yellow", 12, _("50%"), TRUE);
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 3, 1, 1);
    
    label = gtk_label_new (NULL);
    gtk_label_set_xalign (GTK_LABEL(label), 1);
    set_lable_style (label, "gray", 12, _("Network traffic"), TRUE);
    gtk_grid_attach (GTK_GRID (table) ,label, 0, 4, 1, 1);
    
    label = gtk_label_new (NULL);
    set_lable_style (label, "gray", 12, _("Upload: 40Mb/s Download: 100Mb/s"), TRUE);
    gtk_label_set_xalign (GTK_LABEL(label), 0);
    gtk_grid_attach (GTK_GRID (table) ,label, 1, 4, 1, 1);

}

static GObject *
loongson_state_constructor (GType                  type,
                           guint                  n_construct_properties,
                           GObjectConstructParam *construct_properties)
{
    GObject        *obj;
    LoongsonState   *state;

    obj = G_OBJECT_CLASS (loongson_state_parent_class)->constructor (type,
                                      n_construct_properties,
                                      construct_properties);

    state = LOONGSON_STATE (obj);
    loongson_state_fill (state);

    return obj;
}

static void
loongson_state_dispose (GObject *object)
{
    LoongsonState *state;

    state = LOONGSON_STATE (object);
    g_free (state->priv->name);
    G_OBJECT_CLASS (loongson_state_parent_class)->dispose (object);
}

static void
loongson_state_class_init (LoongsonStateClass *klass)
{
    GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->constructor = loongson_state_constructor;
    gobject_class->dispose = loongson_state_dispose;
}

static void
loongson_state_init (LoongsonState *state)
{
    state->priv = loongson_state_get_instance_private (state);
    gtk_orientable_set_orientation (GTK_ORIENTABLE (state), GTK_ORIENTATION_VERTICAL);
    state->priv->name = g_strdup (_("Loongson State"));
}

const char *loongson_state_get_name (LoongsonState *state)
{
    return state->priv->name;
}

GtkWidget *
loongson_state_new (void)
{
    LoongsonState *state;

    state = g_object_new (LOONGSON_TYPE_STATE, NULL);

    return GTK_WIDGET (state);
}
