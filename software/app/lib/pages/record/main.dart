import 'package:flutter/material.dart';

class RecordPage extends StatelessWidget {
  static const title = "Record";

  const RecordPage({super.key});

  @override
  Widget build(BuildContext context) => Scaffold(
        body: Center(
            child: Icon(
          Icons.motion_photos_on,
        )),
      );
}
