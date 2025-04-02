import 'package:dotted_border/dotted_border.dart';
import 'package:flutter/material.dart';

import 'add_new/main.dart';

class AddNewDeviceButton extends StatelessWidget {
  const AddNewDeviceButton({super.key});

  static const double _borderRadius = 7.0;
  static const List<double> _dashPattern = [10, 6];
  static const double _buttonMinSize = 45;
  static const String actionText = 'Add new device';

  @override
  Widget build(BuildContext context) => Center(
      child: DottedBorder(
          color: Colors.blueAccent,
          dashPattern: _dashPattern,
          borderType: BorderType.RRect,
          radius: const Radius.circular(_borderRadius),
          padding: const EdgeInsets.all(0),
          child: ElevatedButton.icon(
            icon: Icon(Icons.add_rounded),
            style: ElevatedButton.styleFrom(
              elevation: 0,
              backgroundColor: Colors.transparent,
              foregroundColor: Colors.blueAccent,
              minimumSize: Size.fromHeight(_buttonMinSize),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(_borderRadius),
              ),
            ),
            onPressed: () => {
              Navigator.of(context).push(
                MaterialPageRoute(
                  builder: (context) => const AddNewPage(),
                ),
              )
            },
            label: const Text(actionText),
          )));
}
