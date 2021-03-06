#include "cchrrombankitemdelegate.h"

#include <QComboBox>
#include <QCompleter>

#include "iprojecttreeviewitem.h"

#include "main.h"

CChrRomBankItemDelegate::CChrRomBankItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* CChrRomBankItemDelegate::createEditor(QWidget* parent,
      const QStyleOptionViewItem& /* option */,
      const QModelIndex& /* index */) const
{
   QComboBox* editor = new QComboBox(parent);

   editor->setEditable(true);

   return editor;
}

void CChrRomBankItemDelegate::setEditorData(QWidget* editor,
      const QModelIndex& index) const
{
   QComboBox* edit = static_cast<QComboBox*>(editor);
   IProjectTreeViewItemIterator iter(nesicideProject->getProject());
   QStringList choices;
   choices.append("<<4KB break>>");
   edit->addItem("<<4KB break>>");
   choices.append("<<2KB break>>");
   edit->addItem("<<2KB break>>");
   choices.append("<<1KB break>>");
   edit->addItem("<<1KB break>>");
   do
   {
      IChrRomBankItem* item = dynamic_cast<IChrRomBankItem*>(iter.current());
      if ( item )
      {
         choices.append(iter.current()->caption());
         edit->addItem(iter.current()->caption());
      }
      iter.next();
   } while ( iter.current() );
   QCompleter* completer = new QCompleter(choices);
   completer->setCompletionMode(QCompleter::PopupCompletion);
   completer->setCompletionPrefix(index.data(Qt::DisplayRole).toString());
   edit->setCompleter(completer);
   edit->showPopup();
   if ( index.data(Qt::DisplayRole).toString() == "<click to add or edit>" )
   {
      edit->setEditText("");
   }
   else
   {
      edit->setEditText(index.data(Qt::DisplayRole).toString());
   }
}

void CChrRomBankItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
      const QModelIndex& index) const
{
   QComboBox* edit = static_cast<QComboBox*>(editor);

   if ( !edit->currentText().isEmpty() )
   {
      model->setData(index, edit->currentText(), Qt::EditRole);
   }
}

void CChrRomBankItemDelegate::updateEditorGeometry(QWidget* editor,
      const QStyleOptionViewItem& option, const QModelIndex& /* index */) const
{
   editor->setGeometry(option.rect);
}
