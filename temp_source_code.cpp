    /*
	* video player
	*/
	QString fileNameQ;
    fileNameQ = QFileDialog::getOpenFileName(this,tr("Open Video"), "", tr("Video Files (*.mp4 *.avi *.mkv)"));
    if(fileNameQ != "") {
        // setup video widget
        QVideoWidget *videoWidget = new QVideoWidget(ui->outputImageView);
        videoWidget->resize(ui->outputImageView->width(), ui->outputImageView->height());
        videoWidget->show();
        QMediaPlayer *player = new QMediaPlayer();
        // volume
        player->setVolume(25);
        // start to play
        player->setVideoOutput(videoWidget);
        QMediaPlaylist *playlist = new QMediaPlaylist(player);
        playlist->addMedia(QUrl::fromLocalFile(fileNameQ));
        playlist->setCurrentIndex(0);
        player->setPlaylist(playlist);
        player->play();
        ui->outputImageViewStop->setEnabled(true);
        connect(ui->outputImageViewStop, SIGNAL(clicked()), player, SLOT(stop()));
        connect(ui->outputImageViewStop, SIGNAL(clicked()), player, SLOT(togglePlay()));
        qDebug() << "Play " << fileNameQ;
    }
    else {
        qDebug() << "Didn't choose any video file.";
    }