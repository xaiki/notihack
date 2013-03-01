#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include <dlfcn.h>
#include <stdio.h>
#include <libnotify/notify.h>
#include <libnotify/notification.h>


struct orig_fn_t
{
    NotifyNotification * (*new)    (const char *, const char *, const char *);
    gboolean             (*update) (NotifyNotification *, const char *, const char *, const char *);
} orig;

char *translate_table = [["hoho", "hola vlad"],
                         ["haha", "hola evita"],
                         ["hehe", "hola peron"]];

NotifyNotification * notify_notification_new            (const char *summary,
                                                         const char *body,
                                                         const char *icon)
{
    if (! strcmp (summary, "hoho")) 
    {
        summary = "hola vlad";
    }

    printf ("hacked new:\n'%s',\n'%s',\n'%s'.\n",
            summary, body, icon);
    return orig.new (summary, body, icon);
}

gboolean            notify_notification_update          (NotifyNotification *notification,
                                                         const char *summary,
                                                         const char *body,
                                                         const char *icon)
{
    printf ("hacked update\n");
    return orig.update (notification, summary, body, icon);
}

void _init (void) {
    printf("Loading hack.\n");

    orig.new    = dlsym (RTLD_NEXT, "notify_notification_new");
    orig.update = dlsym (RTLD_NEXT, "notify_notification_update");

    printf ("hack table:\n");
    printf ("new\t-> %p, %p\n", orig.new, notify_notification_new);
    printf ("update\t-> %p, %p\n", orig.update, notify_notification_update);
}
