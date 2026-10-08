# Jira KRunner Plugin for KDE Plasma 6

A KRunner plugin that matches Jira issue keys (e.g. `PROJ-123`) and opens them directly in your web browser.

### Features
- Matches Jira ticket identifiers in search queries (e.g. `ABC-123`).
- Configurable Jira Base URL via KDE System Settings GUI or CLI.

### Build & Installation

```bash
mkdir -p build
cd build
cmake -DKDE_INSTALL_PLUGINDIR=$(qtpaths6 --plugin-dir) -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
sudo make install
kquitapp6 krunner
```

Alternatively, run `./install.sh`.

### Configuration

#### Via KDE System Settings (GUI)
1. Open **System Settings** -> **Plasma Search** (or run `systemsettings kcm_plasmasearch`).
2. Locate **Jira** under KRunner plugins and click the configure button.
3. Enter your organization's Jira base URL (e.g. `https://your-company.atlassian.net/browse/`) and save.

#### Via Terminal / Command Line
You can set the Jira base URL using `kwriteconfig6`:
```bash
kwriteconfig6 --file krunnerrc --group Runners --group jirarunner --key jiraUrl "https://your-company.atlassian.net/browse/"
```

KRunner automatically reloads its configuration.
