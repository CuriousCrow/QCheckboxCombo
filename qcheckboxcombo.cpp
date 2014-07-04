#include "qcheckboxcombo.h"
#include <QMouseEvent>
#include <QDebug>

QCheckboxCombo::QCheckboxCombo(QWidget *parent) :
  QComboBox(parent)
{
  // set event filter item view
  view()->viewport()->installEventFilter(this);
}

// Event filter prevents combobox from collapsing by mouse click
// and changes "checked" property of items
bool QCheckboxCombo::eventFilter(QObject * watched, QEvent * event)
{
  // filtering only "MouseButtonRelease" event
  if (event->type() == QEvent::MouseButtonRelease)
  {
    qDebug() << "QEvent::MouseButtonRelease";

    // find index of selected item
    QModelIndex ind = view()->indexAt(((QMouseEvent*)event)->pos());
    // get current item status
    bool checked = view()->model()->data(ind,Qt::CheckStateRole).toBool();
    // invert item status
    view()->model()->setData(ind,!checked,Qt::CheckStateRole);
    // block spreading of event
    return true;
  }
  return QObject::eventFilter(watched, event);
}

void QCheckboxCombo::hidePopup()
{
  qDebug() << "hidePopup";

  QStringList values;
  //Building resulting text as a list of checked items
  for (int i=0; i < count(); i++){
    if (itemData(i, Qt::CheckStateRole).toBool()){
      values << itemText(i);
    }
  }
  setCurrentText(values.join(_delimiter));

  QComboBox::hidePopup();
}

void QCheckboxCombo::showPopup()
{
  qDebug() << "showPopup";

  //Setting "checked" property of items that contained in resulting text
  QStringList values = currentText().split(_delimiter);
  for (int i=0; i<count(); i++){
    setItemData(i, values.contains(itemText(i)), Qt::CheckStateRole);
  }
  QComboBox::showPopup();
}
