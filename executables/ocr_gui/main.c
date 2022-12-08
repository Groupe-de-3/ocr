#include <gtk/gtk.h>

typedef struct {
    GtkFileChooserButton *loader_button;
    GtkWidget* save_button;
    GtkWidget* solve_sudoku;
    GtkImage* current_image;


} UserInterface;


void on_file_set(GtkFileChooserButton* self, gpointer user_data)
{
    UserInterface *ui = user_data;
    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(self));
    gtk_image_set_from_file (ui->current_image, filename);
    GdkPixbuf* pix_image = gtk_image_get_pixbuf(ui->current_image);
   
    pix_image = gdk_pixbuf_scale_simple(pix_image, 720, 720, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(ui->current_image, pix_image); 
   
}

char* preprossed(gchar* filename)
{
}

char* solving(gchar* filename)
{
}

  void on_solve(GtkFileChooserButton* self, gpointer user_data)
{
    UserInterface *ui = user_data;
    gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(self));
    char* newfilename = preprossed(filename);
    gtk_image_set_from_file(ui->current_image, newfilename);

    //MAYBE NEED TIME
    
    char* newfilename2 = solving(newfilename);
    gtk_image_set_from_file(ui->current_image, newfilename2);
    
}

// Main function.
//
//
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "duel.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.duel"));
    GtkFileChooserButton* loader_button = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "loader_button")); 
    GtkImage* current_image = GTK_IMAGE(gtk_builder_get_object(builder, "current_image"));

    UserInterface user_init = {
        .loader_button = loader_button,
        .current_image = current_image,
        // .save_button = ,
         .solve_sudoku = ,
    };

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(loader_button, "file-set", G_CALLBACK(on_file_set), &user_init);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
