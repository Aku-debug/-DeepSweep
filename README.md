<h1 align="center">🔧 DEEPSWEEP</h1>

<p align="center">
  <b><i>Clean your system from the DeepSweep – deep uninstall and trace removal for all apps!</i></b><br>
  <b><i>Sisteminizi DeepSweep – tüm uygulamalar için derinlemesine kaldırma ve iz temizleme!</i></b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-blue?style=flat-square">
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey?style=flat-square">
  <img src="https://img.shields.io/badge/Registry-Access-green?style=flat-square">
</p>

---

## 🚀 Features / Özellikler

🌐 **Multilingual README (TR + EN)**  
🗑️ **Full Application Uninstall** – Uses official uninstallers and then removes leftover files & registry keys.  
📁 **Portable App Detection** – Scans drives to find portable software not listed in registry.  
🧹 **Deep Clean** – Removes files from AppData, LocalAppData, and installation folders.  
🧠 **Smart Matching** – Avoids duplicates and skips invalid entries.  
⚙️ **Silent Mode Support** – Adds `/S` or `/quiet` flags to uninstaller commands when supported.  

---

## 🧩 How It Works / Nasıl Çalışır?

🔍 **ShadowUninstaller**, sisteminizde kurulu olan hem **klasik** hem de **portable (taşınabilir)** uygulamaları tarar.  
Kayıt defteri, disk sürücüleri ve `AppData` klasörlerini inceleyerek, uygulamaların tüm izlerini temiz bir şekilde kaldırır.

### Teknik Özellikler:
- `HKEY_LOCAL_MACHINE` ve `HKEY_CURRENT_USER` kayıtları taranır.
- 32-bit ve 64-bit uygulamalar ayrı ayrı kontrol edilir.
- Portable uygulamalar disk sürücülerinde aranır (`Program Files`, `Users`, `AppData` gibi klasörler).
- Varsayılan uninstaller komutları çalıştırılır, ardından uygulama dizinleri ve kullanıcı verileri silinir.

---

## 🖥️ Usage / Kullanım

```bash
🛠️ Derleyin (Visual Studio / g++):
> g++ ShadowUninstaller.cpp -o ShadowUninstaller.exe -lshlwapi -mwindows

🚀 Çalıştırın:
> ShadowUninstaller.exe
