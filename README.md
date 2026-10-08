# Jira KRunner Plugin for KDE Plasma 6

A KRunner plugin that matches Jira issue keys (e.g. `PROJ-123`) and allows searching Jira directly from KRunner.

### Features
- **Direct ticket matching:** Typing ticket identifiers (e.g. `PROJ-123`) opens the ticket directly in the browser.
- **Trigger word support:**
  - `jira <ticket>` (e.g. `jira PROJ-123`): Opens the ticket directly.
  - `jira <search query>` (e.g. `jira login authentication error`): Searches Jira via QuickSearch in your web browser.
- **Configurable Jira Base URL:** Configure via KDE System Settings GUI or CLI.
- **Help integration:** Typing `?` in KRunner documents available Jira runner syntax.

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

### Usage
- `PROJ-123` -> Opens `https://<your-jira>/browse/PROJ-123`
- `jira PROJ-123` -> Opens `https://<your-jira>/browse/PROJ-123`
- `jira memory leak` -> Opens Jira search in browser (`https://<your-jira>/secure/QuickSearch.jspa?searchString=memory+leak`)

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
