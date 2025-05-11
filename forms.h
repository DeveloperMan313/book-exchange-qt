#ifndef FORMS_H
#define FORMS_H

#include "adminauthors.h"
#include "admingenres.h"
#include "adminliterature.h"
#include "adminmenu.h"
#include "login.h"

class Forms {
  public:
    Forms();

    AdminAuthors adminAuthors;
    AdminGenres adminGenres;
    AdminLiterature adminLiterature;
    AdminMenu adminMenu;
    Login login;
};

extern Forms *forms;

#endif // FORMS_H
