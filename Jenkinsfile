pipeline {
    agent none
    environment {
        CUSTOM_CMAKE_OPTIONS = ''
        CTEST_OUTPUT_ON_FAILURE = 1
    }
    stages {
        stage('Build and Deploy') {
            agent {
                label 'jekyll && awscli'
            }
            steps {
                sh '''
                    bundle exec jekyll build
                    aws s3 rm $STABLECODER_S3_SITE/* --recursive
                    aws s3 cp ./_site $STABLECODER_S3_SITE --recursive
                '''
            }
        }
    }
}