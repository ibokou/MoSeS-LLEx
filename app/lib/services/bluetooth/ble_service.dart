import 'dart:io';

import 'package:app/services/bluetooth/exceptions/bluetooth_unsupported_exception.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../main.dart';

class BLEService {
  static late var _subscription;

  static init(WidgetRef ref) async {
    if (await FlutterBluePlus.isSupported == false) {
      throw BluetoothUnsupportedException(message: "");
    }

    // In IOS, only user controls bluetooth enable/disable
    if (!kIsWeb && Platform.isAndroid) {
      await FlutterBluePlus.turnOn();
    }

    _subscription = FlutterBluePlus.adapterState.listen((BluetoothAdapterState state) {
      ref.read(flutterBluePlusStateProvider.notifier).set(state);
    });

    //_subscription.cancel();
    //ref.watch(flutterBluePlusSubscriptionProvider.notifier).set(subscription);
  }

  static scan(timeout, onResult) async {
    _subscription = FlutterBluePlus.onScanResults.listen(onResult);
    FlutterBluePlus.cancelWhenScanComplete(_subscription);
    await FlutterBluePlus.startScan(timeout: timeout);
  }

  static stop() {
    _subscription.cancel();
  }

}