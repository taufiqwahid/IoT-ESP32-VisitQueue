// DELETE LIST AUTH FIREBASE BIAT TIDAK DI HAPUS SATU SATU PADA

setInterval(() => {
  document
    .getElementsByClassName(
      "edit-account-button mat-focus-indicator mat-menu-trigger mat-icon-button mat-button-base",
    )[0]
    .click();
  var buttons = document.getElementsByClassName(
    "mat-focus-indicator mat-menu-item ng-star-inserted",
  );
  buttons.item(buttons.length - 1).click();
  document
    .getElementsByClassName(
      "confirm-button mat-focus-indicator mat-raised-button mat-button-base mat-warn",
    )[0]
    .click();
}, 1000);
