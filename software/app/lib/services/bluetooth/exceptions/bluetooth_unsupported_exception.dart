import 'package:app/util/exceptions/base_exception.dart';

class BluetoothUnsupportedException extends BaseException {
  BluetoothUnsupportedException({super.message});

  @override
  getClassName() => "BluetoothUnsupportedException";
}