// MinecraftNbtViewer
#include "about_dialog.hpp"
#include "version.hpp"

#include <cpp-anvil/version.hpp>

// Qt
#include <QBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QLibraryInfo>
#include <QPushButton>

namespace minecraft {
namespace nbt {

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(QString("About ") + minecraft::nbt::getApplicationName());
    setupUi();
}

AboutDialog::~AboutDialog() = default;

void AboutDialog::setupUi()
{
    // Left side
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(":/icons/grass_block_128x128.png"));

    QVBoxLayout* vLayoutLeft = new QVBoxLayout();
    vLayoutLeft->addWidget(iconLabel);
    vLayoutLeft->addStretch(1);

    // Right side
    QLabel* textLabel = new QLabel(this);
    textLabel->setTextFormat(Qt::TextFormat::RichText);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(textLabel, &QLabel::linkActivated, this, &AboutDialog::openBrowserLink);
    textLabel->setText(makeText());

    QPushButton* okButton      = new QPushButton(tr("Ok"), this);
    QHBoxLayout* hLayoutBottom = new QHBoxLayout();
    hLayoutBottom->addStretch(1);
    hLayoutBottom->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout* vLayoutRight = new QVBoxLayout();
    vLayoutRight->addWidget(textLabel);
    vLayoutRight->addStretch(1);
    vLayoutRight->addLayout(hLayoutBottom);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(vLayoutLeft);
    mainLayout->addLayout(vLayoutRight);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);

    setLayout(mainLayout);
}

QString AboutDialog::makeText()
{
    QString html = R"(
<h1><center>{{AppName}}</center></h1>
<p>
    <b>Version:</b> {{Version}}<br>
    <b>Git Hash:</b> {{GitHash}}<br>
    <b>Repository:</b> <a href="{{Repository}}">Github</a>
</p>
<p>
    <b>cpp-anvil Version:</b> {{CppAnvilVersion}} <a href="{{RepositoryCppAnvil}}">Github</a><br>
    <b>Qt Version:</b> {{QtVersion}}<br>
<p>
    <b>Author:</b> pbmoeller<br>
    <b>License:</b> <a href="{{Repository}}/blob/main/LICENSE">BSD 3-Clause License</a><br>
</p>
)";
    html.replace("{{AppName}}", minecraft::nbt::getApplicationName());
    html.replace("{{Version}}", minecraft::nbt::getVersion());
    html.replace("{{GitHash}}", minecraft::nbt::getGitHash());
    html.replace("{{Repository}}", "https://github.com/pbmoeller/MinecraftNbtViewer");
    html.replace("{{CppAnvilVersion}}", anvil::getCppAnvilVersion());
    html.replace("{{RepositoryCppAnvil}}", "https://github.com/pbmoeller/cpp-anvil");
    html.replace("{{QtVersion}}", qVersion());

    return html;
}

void AboutDialog::openBrowserLink(const QString& link)
{
    QDesktopServices::openUrl(QUrl(link));
}

} // namespace nbt
} // namespace minecraft
