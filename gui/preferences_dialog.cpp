/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include "preferences_dialog.h"
#include <QtWidgets>


PreferencesDialog::PreferencesDialog(QWidget *parent)
: QDialog(parent)
{
  QSettings settings;

  ok_button = new QPushButton("OK", this);  // first button = [enter] button
  cancel_button = new QPushButton("Cancel", this);

  QHBoxLayout *thumbnail_path_layout = new QHBoxLayout;
  thumbnail_path_line_edit = new QLineEdit(
      settings.value("editor/thumbnail_path").toString(), this);
  thumbnail_path_button = new QPushButton("Find...", this);
  thumbnail_path_layout->addWidget(new QLabel("thumbnail path:"));
  thumbnail_path_layout->addWidget(thumbnail_path_line_edit);
  thumbnail_path_layout->addWidget(thumbnail_path_button);
  connect(
      thumbnail_path_button, &QAbstractButton::clicked,
      this, &PreferencesDialog::thumbnail_path_button_clicked);

  QHBoxLayout *bottom_buttons_layout = new QHBoxLayout;
  bottom_buttons_layout->addWidget(cancel_button);
  bottom_buttons_layout->addWidget(ok_button);
  connect(
      ok_button, &QAbstractButton::clicked,
      this, &PreferencesDialog::ok_button_clicked);
  connect(
      cancel_button, &QAbstractButton::clicked,
      this, &QDialog::reject);

  QVBoxLayout *vbox_layout = new QVBoxLayout;
  vbox_layout->addLayout(thumbnail_path_layout);
  // todo: some sort of separator (?)
  vbox_layout->addLayout(bottom_buttons_layout);

  setLayout(vbox_layout);
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::thumbnail_path_button_clicked()
{
  QFileDialog file_dialog(this, "Find Thumbnail Path");
  file_dialog.setFileMode(QFileDialog::ExistingFile);
  file_dialog.setNameFilter("model_list.yaml");
  if (file_dialog.exec() != QDialog::Accepted)
    return;  // user clicked 'cancel'

  const QString model_list_path = file_dialog.selectedFiles().first();
  if (!QFileInfo(model_list_path).exists())
  {
    QMessageBox::critical(
        this,
        "model_list.yaml file does not exist",
        "File does not exist.");
    return;
  }
  thumbnail_path_line_edit->setText(
      QFileInfo(model_list_path).dir().absolutePath());
}

void PreferencesDialog::ok_button_clicked()
{
  if (!thumbnail_path_line_edit->text().isEmpty()) {
    // make sure the path exists
    if (!QFileInfo(thumbnail_path_line_edit->text()).exists()) {
      QMessageBox::critical(
          this,
          "Thumbnail path must exist",
          "Thumbnail path must exist");
      return;
    }
  }

  QSettings settings;
  settings.setValue("editor/thumbnail_path", thumbnail_path_line_edit->text());

  accept();
}