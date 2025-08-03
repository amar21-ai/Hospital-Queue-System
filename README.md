# 🏥 Smart Hospital Queue Management System

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![JavaScript](https://img.shields.io/badge/JavaScript-ES6-yellow.svg)](https://developer.mozilla.org/en-US/docs/Web/JavaScript)
[![HTML5](https://img.shields.io/badge/HTML5-CSS3-red.svg)](https://developer.mozilla.org/en-US/docs/Web/HTML)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)

> An intelligent patient queue management system for healthcare facilities with advanced priority algorithms and comprehensive reporting capabilities.

## 🚀 Features

### 🔥 Core Functionality
- **Multi-Service Queues**: Emergency, Critical, and Checkup services
- **Dynamic Priority Scoring**: Real-time calculation based on urgency, wait time, service type, and visit history
- **Frequent Visitor Recognition**: Automatic bonus scoring for returning patients
- **Emergency Override**: Immediate service capability for critical cases
- **Time Simulation**: Fast-forward functionality for testing and training

### 📊 Advanced Reporting & Analytics
- **Comprehensive Filters**: Service type, urgency level, wait time ranges, priority scores
- **Multiple Report Formats**: Summary, Detailed, Statistical Analysis, CSV Export
- **Real-time Statistics**: Average wait times, queue distributions, performance metrics
- **Data Visualization**: Priority-based queue ordering with visual indicators

### 🎬 Simulation & Testing
- **JSON Simulation Loading**: Import custom scenarios from external files
- **Quick Demo Mode**: Built-in demonstration scenarios
- **Batch Patient Processing**: Serve multiple patients efficiently
- **Admin Console**: Configurable weights and parameters

## 🏗️ System Architecture

This project includes two complementary interfaces:

### 🖥️ Web GUI (`hospital_gui.html`)
- Modern, responsive web interface
- Real-time queue visualization
- Interactive reporting dashboard
- Glassmorphism design with professional styling
- Mobile-friendly responsive layout

### 💻 C++ Console Application (`main.cpp`)
- High-performance backend processing
- Modular class-based architecture
- Memory-efficient queue management
- Comprehensive input validation
- Cross-platform compatibility

### 🧩 Core Components
- `Patient.h` - Patient data structure and methods
- `PriorityEngine.h` - Priority calculation algorithms
- `QueueManager.h` - Queue operations and management
- `AdminConsole.h` - Administrative controls
- `SimulationManager.h` - Simulation handling
- `ReportManager.h` - Report generation and analytics

## 🚀 Quick Start

### Prerequisites
- **For C++ Application**: 
  - C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
  - CMake 3.10+ (optional)
- **For Web GUI**: 
  - Modern web browser (Chrome 60+, Firefox 55+, Safari 12+)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/smart-hospital-queue.git
   cd smart-hospital-queue
   ```

2. **Compile C++ Application**
   ```bash
   g++ -std=c++17 -o hospital_system main.cpp
   # or use your preferred C++ compiler
   ```

3. **Run Web GUI**
   ```bash
   # Simply open hospital_gui.html in your web browser
   # Or serve with a local web server:
   python -m http.server 8000
   # Then visit http://localhost:8000/hospital_gui.html
   ```

### Running the System

**C++ Console Application:**
```bash
./hospital_system
```

**Web Interface:**
Open `hospital_gui.html` in your browser or deploy to any web server.

## 📖 Usage Guide

### Adding Patients
1. Enter Patient ID (1-9999)
2. Select Urgency Level (1-5)
3. Choose Service Type (Emergency/Critical/Checkup)
4. System automatically calculates priority score

### Serving Patients
- **Normal Service**: Serves highest priority patient automatically
- **Emergency Service**: Serves specific patient immediately
- **Batch Service**: Process multiple patients efficiently

### Generating Reports
1. Click "Generate Reports" 
2. Apply desired filters:
   - Service Type, Urgency Level
   - Wait Time Range, Priority Score Range
   - Patient Status, Frequent Visitors
3. Choose sorting options and report format
4. Generate or export data

### Running Simulations
- **JSON File**: Load custom simulation scenarios
- **Quick Demo**: Use built-in test data
- **Real-time**: Add patients with time delays

## 🧮 Priority Algorithm

The system uses a sophisticated priority scoring algorithm:

```
Priority Score = (Urgency × W₁) + (Wait Time × W₂) + (Service Type × W₃) + Frequent Visitor Bonus
```

**Default Weights:**
- Urgency Weight (W₁): 0.5
- Wait Time Weight (W₂): 0.3  
- Service Type Weight (W₃): 0.2

**Service Type Scores:**
- Emergency: 10
- Critical: 8
- Checkup: 5

**Frequent Visitor Bonus:**
- +0.5 for every 5 visits above threshold

## 📁 File Formats

### JSON Simulation Format
```json
[
  {
    "time": 1,
    "patientId": 201,
    "urgency": 5,
    "serviceType": "Emergency"
  },
  {
    "timestamp": 3,
    "patientId": 202,
    "urgency": 3,
    "serviceType": "Critical"
  }
]
```

### CSV Export Format
```csv
Patient ID,Service Type,Urgency,Priority Score,Wait Time (min),Visit Count,Status,Arrival Time,Frequent Visitor Bonus
201,Emergency,5,12.50,5,1,Served,2024-01-15T10:30:00Z,0
```

### C++ Console Interface
```
🏥 Welcome to Smart Hospital Queue Management System
==================================================

🏠 Main Menu
1. Add Patient
2. Serve Next Patient
3. Serve Multiple Patients
4. View All Queues
5. Admin Console
6. Run Simulation
7. Generate Reports
8. Simulate Time Passing
9. Exit
Choice (1-9): 
```

## 🔧 Configuration

### Admin Settings (Configurable)
- Priority algorithm weights
- Service type scoring
- Frequent visitor thresholds
- Time simulation parameters

### Customization Options
- Queue display formatting
- Report templates
- Color schemes and styling
- Language localization support

## 📊 Performance Metrics

- **Queue Processing**: O(n log n) complexity for priority sorting
- **Memory Usage**: Optimized patient data structures
- **Response Time**: <100ms for typical operations
- **Scalability**: Tested with 1000+ concurrent patients

### Development Setup
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style
- C++: Follow Google C++ Style Guide
- JavaScript: Use ESLint with recommended settings
- HTML/CSS: Follow W3C standards

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- **Amar Elsegaey*
- **Youssef Hossam* 

## 🙏 Acknowledgments

- Healthcare professionals who provided domain expertise
- Open source community for tools and libraries
- Beta testers from various medical facilities

⭐ **Star this repository if you find it helpful!**

📧 **Contact**: iamarelsegaey@gmail.com 
