#include "datatableconfig.h"
#include "data_table/data_table_row.h"
#include "qcombobox.h"
#include "ui_datatableconfig.h"
#include "utils/comboboxdelegate.h"

DataTableConfig::DataTableConfig(MainWindow *mw, SelectMenu *sm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTableConfig)
{
    ui->setupUi(this);
    this->mw = mw;

    // Import MW palette
    QPalette pal = mw->palette();
    this->setPalette(pal);

    // Setup open
    QPushButton *add = sm->findChild<QPushButton*>("add");
    connect(add, &QPushButton::clicked, this, [this, mw, sm]{
        if(!this->isVisible() && sm->get_type() == 1)
        {
            this->type = 0;
            this->current_table = new DataTable(mw, "", new QList<DataTableRow>(DEFAULT_TABLE_ROWS));
            update_fields();
            this->show();
        }
    });
    QPushButton *properties = sm->findChild<QPushButton*>("properties");
    connect(properties, &QPushButton::clicked, this, [this, mw, sm]{
        this->type = 1;
        QListWidget *table_list = sm->findChild<QListWidget*>("select_list");
        if(!this->isVisible() && sm->get_type() == 1 && table_list->selectedItems().size() == 1)
        {
            QString table_name = table_list->selectedItems()[0]->text();
            current_table = nullptr;
            for(DataTable *table : *mw->get_tables())
            {
                if(table->get_name().compare(table_name) == 0)
                {
                    current_table = table;
                    break;
                }
            }
            if(current_table == nullptr)
            {
                return;
            }
            this->update_fields();
            this->show();
        }
    });

    // Setup add
    QPushButton *this_add = this->findChild<QPushButton*>("add");
    connect(this_add, &QPushButton::clicked, this, [this]{
        QTableWidget *config_table = this->findChild<QTableWidget*>("config_table");
        config_table->insertRow(config_table->rowCount());
        QComboBox* box = new QComboBox();
        for(QString type : TABLE_ROW_TYPES)
        {
            box->addItem(type);
        }
        int i = config_table->rowCount()-1;
        config_table->setCellWidget(i, 0, box);
        config_table->setItem(i, 1, new QTableWidgetItem(""));
        config_table->setItem(i, 2, new QTableWidgetItem(""));
        config_table->setItem(i, 3, new QTableWidgetItem("4"));
        config_table->setItem(i, 4, new QTableWidgetItem("0"));
    });

    // Setup exit
    QPushButton *exit = this->findChild<QPushButton*>("exit");
    connect(exit, &QPushButton::clicked, this, [this]{
        this->hide();
    });

    // Setup save and close
    QPushButton *save_and_close = this->findChild<QPushButton*>("save_and_close");
    connect(save_and_close, &QPushButton::clicked, this, [this, sm, mw]{
        QSettings *settings = mw->get_settings();
        settings->beginGroup("tables");
        QTableWidget *config_table = this->findChild<QTableWidget*>("config_table");
        current_table->get_rows()->clear();
        for(int i = 0; i < config_table->rowCount(); i++)
        {
            QComboBox* box = qobject_cast<QComboBox*>(config_table->cellWidget(i, 0));
            QString row_type = box->currentText();
            QString row_name = config_table->item(i, 1)->text();
            QString row_unit = config_table->item(i, 2)->text();
            int row_places = config_table->item(i, 3)->text().toInt();
            int row_pspcom_id = config_table->item(i, 4)->text().toInt();
            DataTableRow row(row_type, row_name, row_unit, row_places, row_pspcom_id);
            current_table->get_rows()->append(row);
        }
        QLineEdit *table_name = this->findChild<QLineEdit*>("name");

        // Remove key if editing
        if(this->type == 1)
        {
            settings->remove(current_table->get_name());
        }

        // Set name and add to MW if creating
        current_table->set_name(table_name->text());
        if(this->type == 0)
        {
            mw->add_table(current_table);
        }

        // Update registry
        settings->beginWriteArray(table_name->text());
        for(int i = 0; i < current_table->get_rows()->count(); i++)
        {
            DataTableRow row = current_table->get_rows()->at(i);
            settings->setArrayIndex(i);
            settings->setValue("name", row.get_name());
            settings->setValue("display_name", row.get_display_name());
            settings->setValue("units", row.get_units());
            settings->setValue("decimal_places", row.get_decimal_places());
            settings->setValue("pspcom_id", row.get_pspcom_id());
        }
        settings->endArray();
        settings->endGroup();

        sm->update_fields();

        this->hide();
    });
}

DataTableConfig::~DataTableConfig()
{
    delete ui;
}

void DataTableConfig::update_fields()
{
    QTableWidget *config_table = this->findChild<QTableWidget*>("config_table");
    config_table->clear();
    config_table->setRowCount(0);
    config_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    config_table->setColumnCount(5);
    config_table->setHorizontalHeaderLabels({"Type", "Display Name", "Unit", "Decimal Places", "PSPCOM Sub-device ID"});
    config_table->verticalHeader()->setVisible(false);
    config_table->setItemDelegateForColumn(0, new ComboBoxDelegate());
    QLabel *title = this->findChild<QLabel*>("title");
    if(type == 0)
    {
        this->setWindowTitle("Add New Table...");
        title->setText("New Table");
    }
    else
    {
        this->setWindowTitle("Edit Table");
        title->setText("Edit Table");
    }
    QLineEdit *name = this->findChild<QLineEdit*>("name");
    name->setText(current_table->get_name());

    for(int i = 0; i < current_table->get_rows()->count(); i++)
    {
        config_table->insertRow(config_table->rowCount());
        DataTableRow row = current_table->get_rows()->at(i);

        QComboBox* box = new QComboBox();
        for(QString type : TABLE_ROW_TYPES)
        {
            box->addItem(type);
        }
        box->setCurrentText(row.get_name());
        config_table->setCellWidget(i, 0, box);

        config_table->setItem(i, 1, new QTableWidgetItem(row.get_display_name()));
        config_table->setItem(i, 2, new QTableWidgetItem(row.get_units()));
        config_table->setItem(i, 3, new QTableWidgetItem(QString::number(row.get_decimal_places())));
        config_table->setItem(i, 4, new QTableWidgetItem(QString::number(row.get_pspcom_id())));
    }
}
