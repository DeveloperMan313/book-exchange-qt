#ifndef FORMS_H
#define FORMS_H

#include "adminauthors.h"
#include "admingenres.h"
#include "adminliterature.h"
#include "adminmenu.h"
#include "login.h"
#include "usermenu.h"
#include "usermybooks.h"
#include "useroffers.h"

class Forms {
  public:
    Forms();

    AdminAuthors adminAuthors;
    AdminGenres adminGenres;
    AdminLiterature adminLiterature;
    AdminMenu adminMenu;
    Login login;
    UserMenu userMenu;
    UserMyBooks userMyBooks;
    UserOffers userOffers;
};

extern Forms *forms;

#endif // FORMS_H
