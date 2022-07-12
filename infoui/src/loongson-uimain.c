#include "loongson-window.h"
#include "loongson-utils.h"

#define  BUS_NMAE            "org.loongson.info"

static void acquired_call_back (GDBusConnection *Connection,
                                const gchar     *name,
                                gpointer         data)
{
    GtkWidget *window;
    GError    *error = NULL;
/*
    ss = screen_server_new();
    if (ss == NULL)
    {
        loongson_message_dialog (_("init server"), ERROR,  _("Failed to initialize video source"));
        exit (0);
    }
    
    if (register_screen_server (ss, &error) < 0)
    {
        loongson_message_dialog (_("register server"), ERROR, "register video source interface failed %s", error->message);
        exit (0);
    }
  */  
    window = loongson_window_new ();
    gtk_widget_show_all (window);
}

static void name_lost_call_back (GDBusConnection *connection,
                                 const gchar     *name,
                                 gpointer         data)
{
    loongson_message_dialog (_("Init GDbus"), ERROR, _("dbus name lost call back !!!!"));
    exit (0);
}

int
ls_info_ui (int argc, char **argv)
{
    guint id;

    bindtextdomain (GETTEXT_PACKAGE, LUNAR_CALENDAR_LOCALEDIR);
    textdomain (GETTEXT_PACKAGE);

    gtk_init (&argc, &argv);
    id = g_bus_own_name (G_BUS_TYPE_SESSION,
                         BUS_NMAE,
                         G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT,
                         acquired_call_back,
                         NULL,
                         name_lost_call_back,
                         NULL,
                         NULL);

    gtk_main ();
    return 0;
}
