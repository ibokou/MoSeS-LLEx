import 'package:app/pages/home/devices/button_add_new.dart';
import 'package:flutter/cupertino.dart';

class Devices extends StatelessWidget {
  const Devices({super.key});

  @override
  Widget build(BuildContext context) => Column(
        children: [const AddNewDeviceButton()],
      );
}
